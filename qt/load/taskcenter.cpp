#include "taskcenter.h"
#include <QDir>
#include "buildmusiccore.h"
#include "selectmusicurl.h"
#include "setting.h"
#include "sqlite/sqlite.h"
#include "datacore/dataactive.h"
#include "tlog.h"

TaskCenter::TaskCenter()
{
    m_thread = new QThread;
    m_pool = new QThreadPool;
    this->moveToThread(m_thread);
    m_thread->start();
}

TaskCenter::~TaskCenter()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}


/*
 *遍历文件夹得到所有子文件
 */
void TaskCenter::filterFileInfo(const QStringList& dirPath)
{
    for (const auto & i : dirPath) {
        QDir dir(QUrl(i).toLocalFile());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();
        m_fileInfoList.append(fileList);

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }

    TLog::getInstance()->logLoad(QString("文件数量列表加载完成，共 %1").arg(m_fileInfoList.size()));
}

// 变量文件夹，捕获所有的音乐文件
void TaskCenter::filterFileInfo(const QFileInfoList& fileInfoList)
{
    for (const QFileInfo & i : fileInfoList) {
        QDir dir(i.filePath());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();
        m_fileInfoList.append(fileList);

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }
}

void TaskCenter::selectFile()
{
    long long cell = 0;
    m_pool->setMaxThreadCount(Setting::getInstance()->getMaxThreadNumber());
    m_work = 0;

    for (long long i = 0; i < m_fileInfoList.size(); i+=cell) {
        m_work++;
        cell = 20;
        if (i + cell >= m_fileInfoList.size()) cell = m_fileInfoList.size() - i;
        auto *task = new SelectMusicUrl(m_fileInfoList.mid(i, cell));
        connect(task, &SelectMusicUrl::fileSelected, this, &TaskCenter::appendInfo);

        m_pool->start(task);
    }
    m_fileInfoList.clear();
}

void TaskCenter::loadMedia()
{
    long long cell = 0;
    m_pool->setMaxThreadCount(Setting::getInstance()->getMaxThreadNumber());
    m_work = 0;

    if (m_fileInfoList.empty()) {
        emit DataActive::getInstance()->finish();
        TLog::getInstance()->logLoad("加载完成");
    }

    for (long long i = 0; i < m_fileInfoList.size(); i+=cell) {
        m_work++;
        cell = 20;
        if (i + cell >= m_fileInfoList.size()) cell = m_fileInfoList.size() - i;
        auto *task = new BuildMusicCore(m_fileInfoList.mid(i, cell));
        connect(task, &BuildMusicCore::dataLoaded, this, &TaskCenter::appendMedia);

        m_pool->start(task);
    }
}

/*
 *删除数据
 */
void TaskCenter::clearData()
{
    m_dataList.clear();
    m_albumLSet.clear();
    m_artistSet.clear();
    m_playlistSet.clear();
    m_artistMusicList.clear();
    m_playlistMusicList.clear();
}

void TaskCenter::start()
{
    filterFileInfo(Setting::getInstance()->getSourceList());
    selectFile();
}

void TaskCenter::appendInfo(const QFileInfoList& fileInfoList)
{
    m_fileInfoList.append(fileInfoList);
    m_work--;
    if (m_work == 0 ) {
        QFileInfoList newInfoList;
        TLog::getInstance()->logLoad(QString("sql筛选拥有歌曲文件，共 %1").arg(m_fileInfoList.size()));

        SQLite::getInstance()->selectNewMusic(m_fileInfoList, &newInfoList);
        m_fileInfoList = newInfoList;

        TLog::getInstance()->logLoad(QString("获得筛选未入库的歌曲文件，共 %1").arg(newInfoList.size()));
        loadMedia();
    }
}

void TaskCenter::appendMedia(QList<MediaData> dataList)
{
    m_dataList.append(dataList);
    for (MediaData &i : dataList) {
        MediaData *data = &i;
        for (int j = 0; j < data->artistList.size(); ++j) {
            m_artistSet.insert(data->artistList[j]);
            QPair<QString, QString> pair;
            pair.first = data->url;
            pair.second = data->artistList[j];
            m_artistMusicList.append(pair);
        }
        m_albumLSet.insert(data->album);
        m_playlistSet.insert(data->dir);
        m_dataList.append(*data);
        QPair<QString, QString> pair;
        pair.first = data->url;
        pair.second = data->dir;
        m_playlistMusicList.append(pair);
    }
    m_work--;

    if (m_work == 0) {
        writeDataSQL();
        clearData();
        emit DataActive::getInstance()->finish();
        TLog::getInstance()->logLoad("加载完成");
    }
}

void TaskCenter::writeDataSQL()
{
    SQLite *sql = SQLite::getInstance();
    sql->begin();
    sql->appendAlbum(QStringList(m_albumLSet.begin(), m_albumLSet.end()));
    sql->appendArtist(QStringList(m_artistSet.begin(), m_artistSet.end()));
    sql->appendDirPlayList(QStringList(m_playlistSet.begin(), m_playlistSet.end()));
    sql->appendMusic(m_dataList);
    sql->appendArtistMusic(m_artistMusicList);
    sql->appendPlayListMusic(m_playlistMusicList);
    sql->commit();
}
