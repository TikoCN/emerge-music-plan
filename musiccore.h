#ifndef MUSICCORE_H
#define MUSICCORE_H

#include <QObject>
#include "base/music.h"
#include "base/table.h"
#include "base/album.h"
#include "base/artist.h"

class MusicCore : public QObject
{
    Q_OBJECT
public:
    static MusicCore* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new MusicCore;
        }
    }

    QList<Table *> tableList;       // 播放列表
    QList<Music *> musicList;       // 音乐核心列表
    QList<Album *> albumList;      // 专辑列表
    QList<Artist *> artistList;     // 歌手列表

    QList<Music *> getMusicList() const;

    // 清楚数据
    void clearDate();

    // 写入列表,歌曲数据
    Q_INVOKABLE void writeJsonData();

    // 获得随机列表
    Q_INVOKABLE QList<Album *> getAlbumRandList();
    Q_INVOKABLE QList<Artist *> getArtistRandList();
    Q_INVOKABLE QList<Music *> getMusicRandList();
    // 得到最新加入列表
    Q_INVOKABLE QList<Music *> getNewMusicList();

    // 得到最多播放音乐
    Q_INVOKABLE QList<Music *> getReadMoreList();

    QJsonObject readJsonData();

    //获得音乐核心
    void getMusicCore(QList<Music *>musicList, QList<Table *> tableList, QList<Artist *> artistList, QList<Album *> albumList);

    //新建播放列表
    Q_INVOKABLE void appendTable(QString tableName, bool isDir = false);

    //将歌曲移动到
    Q_INVOKABLE void tableMoveMusic(int orgTableId, int musicId, int aimTalbeId);

    // 将歌曲插入类别
    Q_INVOKABLE void tableInsertMusic(int tableId, Music *music);
    Q_INVOKABLE void tableInsertMusic(int tableId, QList<Music *> musicList);
    Q_INVOKABLE void tableInsertMusic(int tableId, Artist *artist);
    Q_INVOKABLE void tableInsertMusic(int tableId, Album *album);

    QList<Table *> getTableList() const;

    QList<QList<Album *> > getAlbumLineList() const;

    QList<QList<Artist *> > getArtistLineList() const;

    QList<Album *> getAlbumList() const;

    QList<Artist *> getArtistList() const;

private:
    static MusicCore* instance;
    explicit MusicCore(QObject *parent = nullptr);

    Q_PROPERTY(QList<Table *> tableList READ getTableList CONSTANT)

    Q_PROPERTY(QList<Music *> musicList READ getMusicList CONSTANT)

    Q_PROPERTY(QList<Album *> albumList READ getAlbumList CONSTANT FINAL)

    Q_PROPERTY(QList<Artist *> artistList READ getArtistList CONSTANT FINAL)

signals:
    // 列表增加
    void tableAdd();

    // 数据加载完成
    void finishInit();
};

#endif // MUSICCORE_H
