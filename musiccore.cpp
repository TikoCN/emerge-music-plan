#include "musiccore.h"
#include "hosttime.h"

MusicCore::MusicCore(QObject *parent)
    : QObject{parent}
{

}

/*
 * 获得音乐核心
 */
void MusicCore::getMusicCore(QList<Music*> musicList)
{
    QStringList dirs;//本地文件列表
    QList<QList<Music *>> tableMusic;
    //生成播放列表和插入音乐
    for(int i=0; i< musicList.size(); i++){
        musicList[i]->coreId = coreList.size();
        coreList.append(musicList[i]);

        QString dir = musicList[i]->getParentDir();
        int aimTableId = dirs.indexOf(dir);
        if(aimTableId == -1){
            dirs.append(dir);
            aimTableId = dirs.size() - 1;
            QList<Music *> musics;
            tableMusic.append(musics);
        }

        tableMusic[aimTableId].append(musicList[i]);
    }

    //生成本地列表
    for(int i=0; i<dirs.size(); i++){
        appendTable(dirs[i], tableMusic[i], true);
    }

    //清空数据
    HostTime *host = HostTime::getInstance();

    host->clearData();
}

/*
 * 新建播放列表
 */
void MusicCore::appendTable(QString tableName, QList<Music *> musicList, bool isDir)
{
    //判断该列表是否已经存在
    if(!isDir){
        for(int i=0; i<tableList.size(); i++){
            if(tableList[i]->name == tableName && !tableList[i]->isDir){
                return;
            }
        }
    }

    Table* table = new Table(musicList);
    table->name = tableName;
    table->tableId = tableList.size();
    table->isDir = isDir;
    tableList.append(table);

    if(isDir){
        //设置本地列表参数
        table->url = tableName;
        table->name = tableName.split("/").last();
    }

    emit tableAdd();
}

/*
 * 将歌曲移动到
*/
void MusicCore::tableMoveMusic(int orgTableId, int musicId, int aimTalbeId)
{
    Music *core = tableList[orgTableId]->musics[musicId];
    tableList[orgTableId]->removeMusic(musicId);
    tableList[aimTalbeId]->insertMusic(core);
}


QList<Music *> MusicCore::getCoreList() const
{
    return coreList;
}

void MusicCore::clearDate()
{
    while (!tableList.empty()) {
        delete tableList.takeFirst();
    }

    while (!coreList.empty()) {
        delete coreList.takeFirst();
    }
}

QList<Table *> MusicCore::getTableList() const
{
    return tableList;
}
