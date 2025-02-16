#include "hosttime.h"
#include "extralibrary.h"
#include "setting.h"
#include <QPixmap>
#include <QDir>
#include <QCoreApplication>

TaskCell::TaskCell(){
    thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}
TaskCell::~TaskCell(){

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
        //申请控件
        if(!host->getInfoList(&fileList)){
            break;
        }

        QList<Music *> cores;
        QStringList keys;

        for(int i=0; i<fileList.size(); i++){
            QString suffix = fileList[i].suffix();
            if(suffix != "mp3" && suffix != "flac"){
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
        emit musicLoaded(cores, keys);
    }

    emit finishLoad(this);
}

HostTime::HostTime()
{
    buildTaskCellList();
    Setting* seit = Setting::getInstance();
    connect(seit, &Setting::musicCoresChange, this, &HostTime::loadMusicFile);
    semaphore = new QSemaphore(1);
}

/*
 *生成加载单元列表
 */
void HostTime::buildTaskCellList()
{
    Setting* seit = Setting::getInstance();//获得设置指针
    for(int i=0; i<seit->maxThreadNumber; i++){
        TaskCell* cell = new TaskCell;
        taskCellList.append(cell);

        connect(this, &HostTime::startWork, cell, &TaskCell::working);
        connect(cell, &TaskCell::musicLoaded, this, &HostTime::getMusicCoreList);
        connect(cell, &TaskCell::finishLoad, this, &HostTime::cellFinishWork);
    }
}

/*
 *加载音乐文件资源
 */
void HostTime::loadMusicFile(QStringList dirList)
{
    //得到所有音乐文件
    for(int i=0; i<dirList.size(); i++){
        QUrl url(dirList[i]);
        musicFileList.append(getMusicUrl(url.toLocalFile()));
    }

    workNumber = taskCellList.size();
    emit startWork();
}

/*
 *遍历文件夹得到所有子文件
 */
QFileInfoList HostTime::getMusicUrl(QString dirPath)
{
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

bool HostTime::getInfoList(QFileInfoList *list)
{
    semaphore->acquire();
    if(workPos >= musicFileList.size() - 1){
        semaphore->release();
        return false;
    }

    int length = 30;
    int start = workPos;
    if(workPos + length >= musicFileList.size()){
        length = musicFileList.size() - workPos - 1;
    }
    workPos = start + length;
    *list = musicFileList.mid(workPos, length);
    semaphore->release();

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
    workNumber--;
    if(workNumber == 0){
        emit musicsLoaded(coreList, musicKeyList);
    }
}
