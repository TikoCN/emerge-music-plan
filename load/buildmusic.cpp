#include <QThread>
#include "buildmusic.h"
#include "load/taskcenter.h"
#include "musiccore.h"
#include "extralibrary.h"

BuildMusic::BuildMusic(){
    thread = new QThread(this);
    this->moveToThread(thread);
    thread->start();
}
BuildMusic::~BuildMusic(){
    thread->quit();
    thread->wait();
    delete thread;
}

/*
 *加载文件夹中的音乐
 */
void BuildMusic::loadMusicCore()
{
    ExtraLibrary extraLibrary;
    TaskCenter *center = TaskCenter::getInstance();
    MusicCore *core = MusicCore::getInstance();

    while(true){
        QFileInfoList fileList;
        //向 hosttime 申请任务 FileInfoList 数据
        if(!center->getInfoList(&fileList)){
            break;
        }

        QList<Music *> musicList;
        QList<QList<Music *>> tableMusicList;
        tableMusicList.fill(QList<Music *>(), center->tableList.size());

        for(int i=0; i<fileList.size(); i++){
            QString suffix = fileList[i].suffix();
            QStringList list = {"mp3", "flac", "m4a", "aav", "wma", "pcm"};
            if(
                suffix == "jpg" ||
                suffix == "hlrc" ||
                suffix == "lrc" ||
                list.indexOf(suffix) < 0
                ){
                continue;
            }

            //加载音乐数据
            Music *music = new Music;
            music->fromFileInfo(fileList[i]);
            music->moveToThread(core->thread());
            music->readMedia();
            musicList.append(music);

            // 插入列表
            for (int j = 0; j < center->tableList.size(); ++j) {
                if (center->tableList[j]->url == music->getParentDir()) {
                    tableMusicList[j].append(music);
                    break;
                }
            }
        }

        //一次任务加载完成，返回数据
        emit loadedMusicCore(musicList, tableMusicList);
    }

    emit finishMusicCore();
}
