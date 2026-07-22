#include "TaskCenter.h"

#include <QStack>

#include "BuildMusicCore.h"
#include "SelectMusicUrl.h"
#include "Setting.h"
#include "sqlite/Sqlite.h"
#include "datacore/DataActive.h"
#include "Tlog.h"
#include "FileExistCheckTask.h"

TaskCenter::TaskCenter() {
    m_pool = new QThreadPool;
    m_work = 0;
}

TaskCenter::~TaskCenter() {
    m_pool->waitForDone();
    delete m_pool;
}

/*
 * 迭代式遍历目录，避免深层递归导致栈溢出
 */
void TaskCenter::filterFileInfo(const QStringList &dirPath) {
    QStack<QDir> stack;
    for (const auto &path: dirPath) {
        stack.push(QDir(QUrl(path).toLocalFile()));
    }

    while (!stack.isEmpty()) {
        QDir dir = stack.pop();

        // 收集当前目录下的文件
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
        m_fileInfoList.append(dir.entryInfoList());

        // 将子目录压栈
        dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QFileInfo &subDir: dir.entryInfoList()) {
            stack.push(QDir(subDir.filePath()));
        }
    }

    // 日志仅在顶层调用完成后打印一次
    TLog::getInstance().logLoad(QString("文件数量列表加载完成，共 %1").arg(m_fileInfoList.size()));
}

void TaskCenter::selectFile() {
    m_pool->setMaxThreadCount(Setting::getInstance().getMaxThreadNumber());
    m_work = 0;

    const int total = static_cast<int>(m_fileInfoList.size());
    for (int i = 0; i < total; i += BATCH_SIZE_SELECT) {
        m_work++;
        const int count = qMin(BATCH_SIZE_SELECT, total - i);
        auto *    task  = new SelectMusicUrl(m_fileInfoList.mid(i, count));
        // QueuedConnection 确保槽在主线程中执行，保证共享数据访问安全
        connect(task, &SelectMusicUrl::fileSelected, this, &TaskCenter::appendInfo,
                Qt::QueuedConnection);
        m_pool->start(task);
    }
    m_fileInfoList.clear();
}

void TaskCenter::loadMedia() {
    m_pool->setMaxThreadCount(Setting::getInstance().getMaxThreadNumber());
    m_work = 0;

    if (m_fileInfoList.empty()) {
        emit DataActive::getInstance().finish();
        TLog::getInstance().logLoad("加载完成");
        return;
    }

    const int total = static_cast<int>(m_fileInfoList.size());
    for (int i = 0; i < total; i += BATCH_SIZE_LOAD) {
        m_work++;
        const int count = qMin(BATCH_SIZE_LOAD, total - i);
        auto *    task  = new BuildMusicCore(m_fileInfoList.mid(i, count));
        // QueuedConnection 确保槽在主线程中执行，保证共享数据访问安全
        connect(task, &BuildMusicCore::dataLoaded, this, &TaskCenter::appendMedia,
                Qt::QueuedConnection);
        m_pool->start(task);
    }
}

/*
 * 删除数据
 */
void TaskCenter::clearData() {
    m_dataList.clear();
    m_albumSet.clear();
    m_artistSet.clear();
    m_playlistSet.clear();
    m_artistMusicList.clear();
    m_albumMusicList.clear(); // 补上之前遗漏的清理
    m_playlistMusicList.clear();
}

void TaskCenter::start() {
    filterFileInfo(Setting::getInstance().getSourceList());
    selectFile();
}

void TaskCenter::appendInfo(const QFileInfoList &fileInfoList) {
    m_fileInfoList.append(fileInfoList);
    if (--m_work == 0) {
        QFileInfoList newInfoList;
        TLog::getInstance().logLoad(QString("sql筛选拥有歌曲文件，共 %1").arg(m_fileInfoList.size()));

        SQLite::getInstance().selectNewMusic(m_fileInfoList, &newInfoList);
        m_fileInfoList = newInfoList;

        TLog::getInstance().logLoad(QString("获得筛选未入库的歌曲文件，共 %1").arg(newInfoList.size()));
        loadMedia();
    }
}

void TaskCenter::appendMedia(const QList<MediaData> &dataList) {
    // 修复：只插入一次，去除之前循环内的重复 append
    for (const MediaData &data: dataList) {
        static QRegularExpression rx("[,;]+");

        m_dataList.append(data);

        const auto artistList = data.artist.split(rx);
        for (const QString &artist: artistList) {
            m_artistSet.insert(artist);
            m_artistMusicList.append({data.url, artist});
        }

        const auto albumList = data.album.split(rx);
        for (const QString &album: albumList) {
            m_albumSet.insert(album);
            m_albumMusicList.append({data.url, album});
        }

        m_playlistSet.insert(data.dir);
        m_playlistMusicList.append({data.url, data.dir});
    }

    if (--m_work == 0) {
        writeDataSQL();
        clearData();
        emit DataActive::getInstance().finish();
        TLog::getInstance().logLoad("加载完成");
    }
}

/*
 * 纯异步检查文件存在性，去除 QEventLoop 阻塞主线程的问题。
 * 使用原子计数器，所有任务完成后自动 emit fileCheckFinished。
 */
void TaskCenter::checkFileExist(const QList<QPair<int, QString> > &musicDataList) {
    m_pool->setMaxThreadCount(Setting::getInstance().getMaxThreadNumber());

    const int totalTasks = (musicDataList.size() + BATCH_SIZE_CHECK - 1) / BATCH_SIZE_CHECK;

    if (totalTasks == 0) {
        emit fileCheckFinished({});
        return;
    }

    // 使用共享状态在异步回调中汇总结果
    auto *counter    = new std::atomic<int>(0);
    auto *allInvalid = new QList<int>();
    auto *mutex      = new QMutex();

    for (int i = 0; i < musicDataList.size(); i += BATCH_SIZE_CHECK) {
        auto *task = new FileExistCheckTask(musicDataList.mid(i, BATCH_SIZE_CHECK));
        QObject::connect(task, &FileExistCheckTask::checkFinished,
                         this, [this, counter, allInvalid, mutex, totalTasks](const QList<int> &ids) {
                             {
                                 QMutexLocker locker(mutex);
                                 allInvalid->append(ids);
                             }
                             if (++(*counter) >= totalTasks) {
                                 emit fileCheckFinished(*allInvalid);
                                 delete counter;
                                 delete allInvalid;
                                 delete mutex;
                             }
                         },
                         Qt::QueuedConnection);
        m_pool->start(task);
    }
}

void TaskCenter::writeDataSQL() {
    SQLite *sql = &SQLite::getInstance();
    sql->begin();
    sql->appendPort.appendAlbum(QStringList(m_albumSet.begin(), m_albumSet.end()));
    sql->appendPort.appendArtist(QStringList(m_artistSet.begin(), m_artistSet.end()));
    sql->appendPort.appendDirPlayList(QStringList(m_playlistSet.begin(), m_playlistSet.end()));
    sql->appendPort.appendMusic(m_dataList);
    sql->appendPort.appendArtistMusic(m_artistMusicList);
    sql->appendPort.appendAlbumMusic(m_albumMusicList);
    sql->appendPort.appendPlayListMusic(m_playlistMusicList);
    sql->commit();
}
