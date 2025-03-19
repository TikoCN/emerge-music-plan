#include "hosttime.h"
#include "extralibrary.h"
#include "setting.h"
#include "mediaplayer.h"
#include "base.h"
#include "musiccore.h"
#include <QPixmap>
#include <QDir>
#include <QGuiApplication>

TaskCell::TaskCell(){
    thread = new QThread(this);
    this->moveToThread(thread);
    thread->start();
}
TaskCell::~TaskCell(){
    thread->quit();
    thread->wait();
}

/*
 *加载文件夹中的音乐
 */
void TaskCell::loadMusicCore()
{
    ExtraLibrary extraLibrary;
    HostTime *host = HostTime::getInstance();
    MusicCore *core = MusicCore::getInstance();

    while(true){
        QFileInfoList fileList;
        //向 hosttime 申请任务 FileInfoList 数据
        if(!host->getInfoList(&fileList)){
            break;
        }

        QList<Music *> musicList;

        for(int i=0; i<fileList.size(); i++){
            QString suffix = fileList[i].suffix();
            QStringList list = {"mp3", "flac", "m4a", "aav", "wma", "pcm"};
            if(list.indexOf(suffix) < 0){
                continue;
            }

            //加载音乐数据
            Music *music = new Music;
            music->fromFileInfo(fileList[i]);
            music->moveToThread(core->thread());
            extraLibrary.getMedia(music);
            musicList.append(music);
        }

        //一次任务加载完成，返回数据
        emit loadedMusicCore(musicList);
    }

    emit finishMusicCore(this);
}

void TaskCell::loadUserTable()
{
    HostTime *host = HostTime::getInstance();
    QStringList musicNameList = host->musicNameList;
    QList<Music *> musicList = host->musicList;

    while(true){
        QStringList musicIdList;
        int tableId = 0;
        //向 hosttime 申请任务 FileInfoList 数据
        if (!host->getUserTableTask(&musicIdList, &tableId)) {
            break;
        }

        QList<Music *> aimMusicList;
        for (int i = 0; i < musicIdList.size(); ++i) {
            int id = musicIdList[i].toInt();
            QString baseName = musicNameList[id];

            // 找到目标歌曲
            for (int j = 0; j < musicList.size(); ++j) {
                if(musicList[j]->getBaseName() == baseName){
                    aimMusicList.append(musicList[j]);
                    break;
                }
            }
        }


        //一次任务加载完成，返回数据
        emit loadedUserTable(musicList, tableId);
    }

    emit finishUserTable(this);
}

HostTime::HostTime()
{
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();

    MediaPlayer* play = MediaPlayer::getInstance();
    connect(this, &HostTime::initData, play, &MediaPlayer::clearData);
    connect(play, &MediaPlayer::finishClearData, this, &HostTime::loadMusicFile);

    Setting* seit = Setting::getInstance();//获得设置指针
    connect(seit, &Setting::loadMusics, this, &HostTime::buildHostTime);
}

HostTime::~HostTime()
{
}

/*
 *生成加载单元列表
 */
void HostTime::buildHostTime()
{
    //创建信号量
    semaphore = new QSemaphore(1);

    Setting* seit = Setting::getInstance();//获得设置指针
    for(int i=0; i<seit->maxThreadNumber; i++){
        TaskCell* cell = new TaskCell;
        taskCellList.append(cell);

        cell->connect(this, &HostTime::startMusicCore, cell, &TaskCell::loadMusicCore);
        cell->connect(this, &HostTime::startUserTable, cell, &TaskCell::loadUserTable);

        cell->connect(cell, &TaskCell::loadedMusicCore, this, &HostTime::getMusicCoreList);
        cell->connect(cell, &TaskCell::loadedUserTable, this, &HostTime::getUserTableMusic);

        cell->connect(cell, &TaskCell::finishMusicCore, this, &HostTime::finishMusicCore);
        cell->connect(cell, &TaskCell::finishUserTable, this, &HostTime::finishUserTable);
    }

    emit initData();
}

/*
 *加载音乐文件资源
 */
void HostTime::loadMusicFile()
{
    Setting* seit = Setting::getInstance();//获得设置指针
    QStringList dirList = seit->sourceList;

    //得到所有音乐文件
    for(int i=0; i<dirList.size(); i++){
        QUrl url(dirList[i]);
        musicFileList.append(getMusicUrl(url.toLocalFile()));
    }

    workPos = 0;//重置工作位置
    workNumber = taskCellList.size();//设置工作单元数
    emit startMusicCore();
}

void HostTime::loadUserTable()
{
    MusicCore *core = MusicCore::getInstance();
    QJsonObject data = core->readJsonData();
    QJsonObject music = data.value("music").toObject();
    QJsonObject table = data.value("table").toObject();

    // 得到旧的音乐对照表
    musicNameList = music.value("music").toString().split("|");

    int size = table.value("size").toInt();
    for (int i = 0; i < size; ++i) {
        QJsonObject tableJson = table.value(QString::number(i)).toObject();

        // 读取本地列表数据, 并排序
        if (tableJson.value("isDir").toBool()) {
            for (int j = 0; j < tableList.size(); ++j) {
                if (tableList[j]->url == tableJson.value("url")) {
                    tableList[j]->sortMusic(tableJson.value("sort").toInt());
                    break;
                }
            }
        }
        else {
            // 创建列表
            Table *table = new Table;
            table->tableId = tableList.size();
            tableList.append(table);
            table->name = tableJson.value("name").toString();
            table->isDir = tableJson.value("isDir").toBool();
            table->sortMusic(tableJson.value("sort").toInt());
            table->moveToThread(core->thread());

            // 得到歌曲附录
            QStringList musicIdList = tableJson.value("music").toString().split("|");
            tableMusic.append(musicIdList);
        }
    }

    workNumber = taskCellList.size();
    workPos = 0;
    tableId = dirTableSize;
    emit startUserTable();
}

/*
 *遍历文件夹得到所有子文件
 */
QFileInfoList HostTime::getMusicUrl(QString dirPath)
{
    if(dirPath.isEmpty()){
        return QFileInfoList();//返回空列表
    }

    QDir dir(dirPath);
    QFileInfoList allFileList;
    QFileInfoList childDirList;

    //得到子文件夹
    dir.setFilter(QDir::Dirs |QDir::NoDotAndDotDot);
    childDirList = dir.entryInfoList();

    //得到子文件
    dir.setFilter(QDir::Files |QDir::NoDotAndDotDot);
    allFileList = dir.entryInfoList();
    for(int i=0; i<childDirList.size(); i++){
        allFileList.append(getMusicUrl(childDirList[i].filePath()));
    }

    return allFileList;
}

/*
 * 申请分配文件列表
 */
bool HostTime::getInfoList(QFileInfoList *list)
{
    semaphore->acquire();                      //请求读写
    //查看任务是否完成，已经任务队列是否为空
    if(workPos > musicFileList.size() - 1 || musicFileList.size() <= 0){
        semaphore->release();
        return false;
    }

    int length = 30;
    int start = workPos;
    if(workPos + length >= musicFileList.size()){
        length = musicFileList.size() - workPos;
    }
    *list = musicFileList.mid(start, length);
    workPos = start + length;
    semaphore->release();                      //释放读写

    return true;
}

bool HostTime::getUserTableTask(QStringList *musicNameList, int *aim)
{
    semaphore->acquire();                      //请求读写
    QStringList musicIdList;
    while (true) {
        // 任务结束
        if (tableId == tableList.size()) {
            semaphore->release();
            return false;
        }

        musicIdList = tableMusic[tableId - dirTableSize];

        //查看任务是否完成，已经任务队列是否为空
        if(workPos > musicIdList.size() - 1 || musicIdList.size() <= 0){
            semaphore->release();
            tableId++;
        }
    }

    int length = 30;
    int start = workPos;
    if(workPos + length >= musicIdList.size()){
        length = musicIdList.size() - workPos;
    }

    // 返回数据
    *musicNameList = musicIdList.mid(start, length);
    *aim = tableId;

    // 移动工作点
    workPos = start + length;
    semaphore->release();

    return true;
}

/*
 *获得加载好的音乐数据
 */
void HostTime::getMusicCoreList(QList<Music *> musicList)
{
    MusicCore *core = MusicCore::getInstance();

    // 遍历获得的音乐列表
    for (int i = 0; i < musicList.size(); ++i) {
        musicList[i]->coreId = this->musicList.size();
        this->musicList.append(musicList[i]);
        QString dir = musicList[i]->getParentDir();

        // 寻找是否建立该列表
        bool isNoFind = true;
        for (int j = 0; j < tableList.size(); ++j) {
            if (dir == tableList[j]->url) {
                tableList[j]->insertMusic(musicList[i]);
                isNoFind = false;
                break;
            }
        }

        // 没有发现目标列表
        if (isNoFind) {
            Table *table = new Table;
            table->tableId = tableList.size();
            tableList.append(table);
            table->name = dir.split("/").last();
            table->url = dir;
            table->isDir = true;
            table->moveToThread(core->thread());
        }
    }

    dirTableSize = tableList.size();
}

void HostTime::getUserTableMusic(QList<Music *> musicList, int tableId)
{
    if (tableId < 0 || tableId >= tableList.size()){
        return;
    }
    // 插入列表
    tableList[tableId]->insertMusic(musicList);
}

void HostTime::finishMusicCore(TaskCell *cell)
{
    //计算当前工作单元数
    workNumber--;
    if(workNumber == 0){
        loadUserTable();
    }
}

void HostTime::finishUserTable(TaskCell *cell)
{
    //计算当前工作单元数
    workNumber--;
    if(workNumber == 0){
        QString t = tr("加载音乐文件完成，加载了 ") + QString::number(musicList.size()) + tr(" 个音乐文件");
        emit Base::getInstance()->sendMessage(t, 0);
        emit musicsLoaded(musicList, tableList);
    }
}

/*
 *删除数据
 */
void HostTime::clearData()
{
    this->musicList.clear();
    this->musicFileList.clear();

    //从头删除到尾
    while (!taskCellList.empty()) {
        delete taskCellList.takeFirst();
    }

    //删除信号量
    delete semaphore;
}
