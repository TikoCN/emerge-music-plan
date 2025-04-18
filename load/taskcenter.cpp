#include "taskcenter.h"
#include <QDir>
#include <QUrl>
#include "buildmusiccore.h"
#include "selectmusicurl.h"
#include "setting.h"
#include "sqlite/sqlite.h"

TaskCenter::TaskCenter()
{
    _thread = new QThread;
    _pool = new QThreadPool;
    this->moveToThread(_thread);
    _thread->start();
}

TaskCenter::~TaskCenter()
{
    _thread->quit();
    _thread->wait();
    delete _thread;
}


/*
 *遍历文件夹得到所有子文件
 */
void TaskCenter::filterFileInfo(QStringList dirPath)
{
    for (int i = 0; i < dirPath.size(); ++i) {
        QDir dir(QUrl(dirPath[i]).toLocalFile());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();
        _fileInfoList.append(fileList);

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }
}

// 变量文件夹，捕获所有的音乐文件
void TaskCenter::filterFileInfo(QFileInfoList dirs)
{
    for (int i = 0; i < dirs.size(); ++i) {
        QDir dir(dirs[i].filePath());

        //得到子文件
        dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
        QFileInfoList fileList = dir.entryInfoList();
        _fileInfoList.append(fileList);

        //得到子文件夹
        dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
        filterFileInfo(dir.entryInfoList());
    }
}

void TaskCenter::selectFile()
{
    int cell = 0;
    _pool->setMaxThreadCount(Setting::getInstance()->getMaxThreadNumber());
    _work = 0;

    for (int i = 0; i < _fileInfoList.size(); i+=cell) {
        _work++;
        cell = 20;
        if (i + cell >= _fileInfoList.size()) cell = _fileInfoList.size() - i;
        SelectMusicUrl *task = new SelectMusicUrl(_fileInfoList.mid(i, cell));
        task->connect(task, &SelectMusicUrl::fileSelected, this, &TaskCenter::appendInfo);

        _pool->start(task);
    }
    _fileInfoList.clear();
}

void TaskCenter::loadMedia()
{
    int cell = 0;
    _pool->setMaxThreadCount(Setting::getInstance()->getMaxThreadNumber());
    _work = 0;

    for (int i = 0; i < _fileInfoList.size(); i+=cell) {
        _work++;
        cell = 20;
        if (i + cell >= _fileInfoList.size()) cell = _fileInfoList.size() - i;
        BuildMusicCore *task = new BuildMusicCore(_fileInfoList.mid(i, cell));
        task->connect(task, &BuildMusicCore::dataLoaded, this, &TaskCenter::appendMedia);

        _pool->start(task);
    }
}

/*
 *删除数据
 */
void TaskCenter::clearData()
{
    _dataList.clear();
    _albumLSet.clear();
    _artistSet.clear();
    _playlistSet.clear();
    _artistMusicList.clear();
    _playlistMusicList.clear();
}

void TaskCenter::start()
{
    filterFileInfo(Setting::getInstance()->sourceList);
    selectFile();
}

void TaskCenter::appendInfo(QFileInfoList fileInfoList)
{
    _fileInfoList.append(fileInfoList);
    _work--;
    if (_work == 0 ) {
        QFileInfoList newInfoList;
        SQLite::getInstance()->selectNewMusic(_fileInfoList, &newInfoList);
        _fileInfoList = newInfoList;
        loadMedia();
    }
}

void TaskCenter::appendMedia(QList<MediaData> dataList)
{
    _dataList.append(dataList);
    for (int i = 0; i < dataList.size(); ++i) {
        MediaData *data = &dataList[i];
        for (int j = 0; j < data->artistList.size(); ++j) {
            _artistSet.insert(data->artistList[j]);
            QPair<QString, QString> pair;
            pair.first = data->url;
            pair.second = data->artistList[j];
            _artistMusicList.append(pair);
        }
        _albumLSet.insert(data->album);
        _playlistSet.insert(data->dir);
        _dataList.append(*data);
        QPair<QString, QString> pair;
        pair.first = data->url;
        pair.second = data->dir;
        _playlistMusicList.append(pair);
    }
    _work--;
    if (_work == 0) {
        writeDataSQL();
        clearData();
        emit finish();
    }
}

void TaskCenter::writeDataSQL()
{
    SQLite *sql = SQLite::getInstance();
    sql->begin();
    sql->appendAlbum(QStringList(_albumLSet.begin(), _albumLSet.end()));
    sql->appendArtist(QStringList(_artistSet.begin(), _artistSet.end()));
    sql->appendDirTable(QStringList(_playlistSet.begin(), _playlistSet.end()));
    sql->appendMusic(_dataList);
    sql->appendArtistMusic(_artistMusicList);
    sql->appendListMusic(_playlistMusicList);
    sql->commit();
}
