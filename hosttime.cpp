#include "hosttime.h"
#include "extralibrary.h"
#include "setting.h"
#include "mediaplayer.h"
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>

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
void TaskCell::working(){
    ExtraLibrary extraLibrary;
    Setting *seit = Setting::getInstance();
    HostTime *host = HostTime::getInstance();
    QJsonObject *coreJson = seit->coreJson;
    QStringList *musicKeyList = &seit->musicKeyList;

    while(true){
        QFileInfoList fileList;
        //向 hosttime 申请任务 FileInfoList 数据
        if(!host->getInfoList(&fileList)){
            break;
        }

        QList<Music *> cores;
        QStringList keys;

        for(int i=0; i<fileList.size(); i++){
            QString suffix = fileList[i].suffix();
            QStringList list = {"mp3", "flac", "m4a", "aav", "wma", "pcm"};
            if(list.indexOf(suffix) < 0){
                continue;
            }

            //加载音乐数据
            Music *core = new Music;
            core->fromFileInfo(fileList[i]);
            extraLibrary.getMedia(core);
            cores.append(core);
            keys.append(core->getKey());

            int aim = musicKeyList->indexOf(core->getKey());
            if(aim != -1){
                core->fromJson(coreJson->value(QString::number(aim)).toObject());
            }

            //移动到主线程
            core->moveToThread(QCoreApplication::instance()->thread());
        }

        //一次任务加载完成，返回数据
        emit musicLoaded(cores, keys);
    }

    emit finishLoad(this);
}

HostTime::HostTime()
{
    MediaPlayer* play = MediaPlayer::getInstance();
    connect(this, &HostTime::initData, play, &MediaPlayer::clearData);
    connect(play, &MediaPlayer::finishClearData, this, &HostTime::loadMusicFile);

    Setting* seit = Setting::getInstance();//获得设置指针
    connect(seit, &Setting::loadMusics, this, &HostTime::buildHostTime);
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

        cell->connect(this, &HostTime::startWork, cell, &TaskCell::working);
        cell->connect(cell, &TaskCell::musicLoaded, this, &HostTime::getMusicCoreList);
        cell->connect(cell, &TaskCell::finishLoad, this, &HostTime::cellFinishWork);
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
    emit startWork();
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

/*
 *获得加载好的音乐数据
 */
void HostTime::getMusicCoreList(QList<Music *> coreList, QStringList musicKeyList)
{
    this->musicKeyList.append(musicKeyList);
    this->coreList.append(coreList);
}

void HostTime::cellFinishWork(TaskCell *cell)
{
    //计算当前工作单元数
    workNumber--;
    if(workNumber == 0){
        qDebug()<<"work success, loaded music files number : " + QString::number(coreList.size());
        emit musicsLoaded(coreList, musicKeyList);
    }
}

/*
 *删除数据
 */
void HostTime::clearData()
{
    this->coreList.clear();
    this->musicKeyList.clear();
    this->musicFileList.clear();

    //从头删除到尾
    while (!taskCellList.empty()) {
        delete taskCellList.takeFirst();
    }

    //删除信号量
    delete semaphore;
}
