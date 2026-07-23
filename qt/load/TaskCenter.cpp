#include "TaskCenter.h"

#include "FileExistCheckTask.h"
#include "FileScanner.h"
#include "MediaParser.h"
#include "sqlite/Sqlite.h"
#include "Setting.h"
#include "Tlog.h"
#include "datacore/DataActive.h"

TaskCenter::TaskCenter()
    : m_pool(QThreadPool::globalInstance()) {
}

TaskCenter::~TaskCenter() = default;

void TaskCenter::clearData() {
    m_fileInfoList.clear();
    m_dataList.clear();
}

void TaskCenter::startLoadFile() {
    m_pool->setMaxThreadCount(Setting::getInstance().getMaxThreadNumber());
    m_work = 0;
    clearData();

    const QStringList &sourceList = Setting::getInstance().getSourceList();
    scanSubDirectories(sourceList);
}

void TaskCenter::startBootSequence() {
    // 第一步：启动无效数据清理
    SQLite::getInstance().startClearInvalidData();
}

void TaskCenter::onInvalidDataCleared() {
    // 第二步：无效数据清理完成后，启动音乐加载
    startLoadFile();
}

void TaskCenter::scanSubDirectories(const QStringList &subDirPaths) {
    for (const QString &dirPath: subDirPaths) {
        m_work++;
        auto *scanner = new FileScanner(dirPath);
        connect(scanner, &FileScanner::scanFinished, this, &TaskCenter::onScanFinished,
                Qt::QueuedConnection);
        m_pool->start(scanner);
    }
}

void TaskCenter::onScanFinished(QFileInfoList fileInfoList, QStringList subDirPaths) {
    // 累加当前目录的文件列表
    m_fileInfoList.append(fileInfoList);

    // 如果有子目录，继续并行扫描
    if (!subDirPaths.isEmpty()) {
        scanSubDirectories(subDirPaths);
    }

    // 所有扫描器完成
    if (--m_work == 0) {
        TLog::getInstance().logLoad(QString("文件扫描完成，共 %1").arg(m_fileInfoList.size()));

        QFileInfoList newInfoList;
        SQLite::getInstance().selectNewMusic(m_fileInfoList, &newInfoList);
        m_fileInfoList = newInfoList;

        TLog::getInstance().logLoad(QString("筛选未入库的歌曲文件，共 %1").arg(newInfoList.size()));
        loadMedia();
    }
}

void TaskCenter::loadMedia() {
    m_work = 0;

    for (int i = 0; i < m_fileInfoList.size(); i += BATCH_SIZE_LOAD) {
        m_work++;
        int           end   = qMin(i + BATCH_SIZE_LOAD, m_fileInfoList.size());
        QFileInfoList batch = m_fileInfoList.mid(i, end - i);

        auto *parser = new MediaParser(batch);
        connect(parser, &MediaParser::parseFinished, this, &TaskCenter::onParseFinished,
                Qt::QueuedConnection);
        m_pool->start(parser);
    }
}

void TaskCenter::onParseFinished(QList<MediaData> dataList) {
    m_dataList.append(dataList);

    if (--m_work == 0) {
        SQLite::getInstance().insertMediaData(m_dataList);
        clearData();
        emit DataActive::getInstance().finish();
        TLog::getInstance().logLoad("加载完成");
    }
}

void TaskCenter::checkFileExist(const QList<QPair<int, QString> > &musicDataList) {
    m_work = 0;
    m_allInvalidIds.clear();

    for (int i = 0; i < musicDataList.size(); i += BATCH_SIZE_CHECK) {
        m_work++;
        int   end  = qMin(i + BATCH_SIZE_CHECK, musicDataList.size());
        auto *task = new FileExistCheckTask(musicDataList.mid(i, end - i));
        connect(task, &FileExistCheckTask::checkFinished, this,
                [this](const QList<int> &invalidIds) {
                    m_allInvalidIds.append(invalidIds);
                    if (--m_work == 0) {
                        emit fileCheckFinished(m_allInvalidIds);
                    }
                }, Qt::QueuedConnection);
        m_pool->start(task);
    }
}
