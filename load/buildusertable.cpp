#include "buildusertable.h"
#include <QThread>
#include "load/taskcenter.h"

BuildUserTable::BuildUserTable(){
    thread = new QThread(this);
    this->moveToThread(thread);
    thread->start();
}
BuildUserTable::~BuildUserTable(){
    thread->quit();
    thread->wait();
    delete thread;
}

void BuildUserTable::loadUserTable()
{
    TaskCenter *center = TaskCenter::getInstance();
    QStringList musicNameList = center->musicNameList;
    QList<Music *> musicList = center->musicList;

    while(true){
        QStringList musicIdList;
        int tableId = -1;
        //向 hosttime 申请任务 FileInfoList 数据
        if (!center->getUserTableTask(&musicIdList, &tableId)) {
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
        emit loadedUserTable(aimMusicList, tableId);
    }

    emit finishUserTable();
}
