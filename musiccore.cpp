#include "musiccore.h"
#include "base.h"
#include "load/taskcenter.h"
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

MusicCore::MusicCore(QObject *parent)
    : QObject{parent}
{

}

/*
 * 获得音乐核心
 */
void MusicCore::getMusicCore(QList<Music *>musicList, QList<Table *> tableList, QList<Artist *> artistList, QList<Album *> albumList)
{
    this->musicList = musicList;
    this->tableList = tableList;
    this->albumList = albumList;
    this->artistList = artistList;

    emit finishInit();

    //清空数据
    TaskCenter *host = TaskCenter::getInstance();
    host->clearData();
}

/*
 * 新建播放列表
 */
void MusicCore::appendTable(QString tableName, bool isDir)
{
    //判断该列表是否已经存在
    if(!isDir){
        for(int i=0; i<tableList.size(); i++){
            if(tableList[i]->name == tableName && !tableList[i]->isDir){
                return;
            }
        }
    }

    Table* table = new Table();
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
    tableList[aimTalbeId]->appendMusic(core);
}

void MusicCore::tableInsertMusic(int tableId, Music *music)
{
    if (tableId < 0 || tableId >= tableList.size() || music == nullptr) {
        QString msg = tr("列表") +" "+ QString::number(tableId) +" "+ tr("插入歌曲失败");
        Base::getInstance()->sendMessage(msg, 0);
        return;
    }
    tableList[tableId]->appendMusic(music);
}

void MusicCore::tableInsertMusic(int tableId, QList<Music *> musicList)
{
    if (tableId < 0 || tableId >= tableList.size()) {
        QString msg = tr("列表") +" "+ QString::number(tableId) +" "+ tr("插入歌曲失败");
        Base::getInstance()->sendMessage(msg, 0);
        return;
    }

    tableList[tableId]->appendMusic(musicList);
}

void MusicCore::tableInsertMusic(int tableId, Artist *artist)
{
    if (tableId < 0 || tableId >= tableList.size() || artist == nullptr) {
        QString msg = tr("列表") +" "+ QString::number(tableId) +" "+ tr("插入歌曲失败");
        Base::getInstance()->sendMessage(msg, 0);
        return;
    }
    tableList[tableId]->appendMusic(artist->musicList);
}

void MusicCore::tableInsertMusic(int tableId, Album *album)
{
    if (tableId < 0 || tableId >= tableList.size() || album == nullptr) {
        QString msg = tr("列表") +" "+ QString::number(tableId) +" "+ tr("插入歌曲失败");
        Base::getInstance()->sendMessage(msg, 0);
        return;
    }
    tableList[tableId]->appendMusic(album->musicList);
}

QList<Album *> MusicCore::getAlbumList() const
{
    return albumList;
}

QList<Artist *> MusicCore::getArtistList() const
{
    return artistList;
}

QList<Music *> MusicCore::getMusicList() const
{
    return musicList;
}

void MusicCore::clearDate()
{
    while (!tableList.empty()) {
        delete tableList.takeFirst();
    }

    while (!musicList.empty()) {
        delete musicList.takeFirst();
    }
}

void MusicCore::writeJsonData()
{
    QFile dataFile(QDir::currentPath() + "/data.json");
    // 打开文件失败，退出读写
    if(!dataFile.open(QIODevice::Text | QIODevice::WriteOnly)){
        dataFile.close();
        return;
    }
    QJsonObject writeData;

    // 歌曲信息
    QJsonObject musicJson;
    QStringList musicBaseNameList;
    musicJson.insert("size", musicList.size());
    for (int i = 0; i < musicList.size(); ++i) {
        musicBaseNameList.append(musicList[i]->getBaseName());
    }
    musicJson.insert("music", musicBaseNameList.join("|"));
    writeData.insert("music", musicJson);

    // 列表信息
    QJsonObject tableJosn;
    for (int i = 0; i < tableList.size(); ++i) {
        QJsonObject cell;
        cell.insert("isDir", tableList[i]->isDir);
        cell.insert("sort", tableList[i]->getSort());

        Table *table = tableList[i];
        // 生成列表单元数据
        if (table->isDir) {
            cell.insert("url", table->url);
        }
        else {
            QStringList musicList;
            for (int j = 0; j < table->musics.size(); ++j) {
                musicList.append(QString::number(table->musics[j]->coreId));
            }
            cell.insert("music", musicList.join("|"));
            cell.insert("name", table->name);
        }
        // 插入
        tableJosn.insert(QString::number(i), cell);
        tableJosn.insert("size", tableList.size());
    }
    writeData.insert("table", tableJosn);

    // 执行写入文件
    QJsonDocument doc(writeData);
    dataFile.write(doc.toJson(QJsonDocument::Indented));
    dataFile.close();
}

QList<Album *> MusicCore::getAlbumRandList()
{
    if(albumList.size() < 15){
        return albumList;
    }

    QList<Album *> list;
    QList<int> aimList = Base::getInstance()->getRandNumber(0, albumList.size(), 15);
    for (int i = 0; i < aimList.size(); i++) {
        if (i < albumList.size()) {
            list.append(albumList[i]);
        }
    }
    return list;
}

QList<Artist *> MusicCore::getArtistRandList()
{
    if(artistList.size() < 15){
        return artistList;
    }

    QList<Artist *> list;
    QList<int> aimList = Base::getInstance()->getRandNumber(0, artistList.size(), 15);
    for (int i = 0; i < aimList.size(); i++) {
        if (i < artistList.size()) {
            list.append(artistList[i]);
        }
    }
    return list;
}

QList<Music *> MusicCore::getMusicRandList()
{
    if(musicList.size() < 15){
        return musicList;
    }

    QList<Music *> list;
    QList<int> aimList = Base::getInstance()->getRandNumber(0, musicList.size(), 15);
    for (int i = 0; i < aimList.size(); i++) {
        if (i < musicList.size()) {
            list.append(musicList[i]);
        }
    }
    return list;
}

QList<Music *> MusicCore::getNewMusicList()
{
    if(musicList.size() < 45){
        return musicList;
    }

    QList<Music *> list = musicList;

    std::partial_sort(list.begin(), list.begin() + 45, list.end(), [](Music *a, Music *b){
        return a->endTime > b->endTime;
    });
    return list.mid(0, 45);
}

QList<Music *> MusicCore::getReadMoreList()
{
    if(musicList.size() < 45){
        return musicList;
    }

    QList<Music *> list = musicList;

    std::partial_sort(list.begin(), list.begin() + 45, list.end(), [](Music *a, Music *b){
        return a->playNumber > b->playNumber;
    });
    return list.mid(0, 45);
}

QJsonObject MusicCore::readJsonData()
{
    QFile dataFile(QDir::currentPath() + "/data.json");
    // 打开文件失败，退出读写
    if(!dataFile.open(QIODevice::Text | QIODevice::ReadOnly)){
        dataFile.close();
        return QJsonObject();
    }

    // 执行写入文件
    QJsonDocument doc;
    doc = doc.fromJson(dataFile.readAll());
    dataFile.close();
    return doc.object();
}

QList<Table *> MusicCore::getTableList() const
{
    return tableList;
}
