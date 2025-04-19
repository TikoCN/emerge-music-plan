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

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    //新建播放列表
    Q_INVOKABLE void appendTable(QString name);

    // //将歌曲移动到
    // Q_INVOKABLE void tableMoveMusic(int orgTableId, int musicId, int aimTalbeId);

    // // 将歌曲插入类别
    // Q_INVOKABLE void tableInsertMusic(int tableId, int musicId);
    // Q_INVOKABLE void tableInsertArtist(int tableId, int artistId);
    // Q_INVOKABLE void tableInsertAlbum(int tableId, int albumId);

    Q_INVOKABLE Table *getTable(int id);

    Q_INVOKABLE Album *getAlbum(int id);

    Q_INVOKABLE Artist *getArtist(int id);

    Q_INVOKABLE Music *getMusic(int id);
    Q_INVOKABLE QList<Music *> getMusic(QList<int > idList);

private:
    static MusicCore *instance;
    explicit MusicCore(QObject *parent = nullptr);

    QHash<int, Table *> m_tableHash;       // 播放列表
    QHash<int, Music *> m_musicHash;       // 音乐核心列表
    QHash<int, Album *> m_albumHash;      // 专辑列表
    QHash<int, Artist *> m_artistHash;     // 歌手列表
signals:
    void finish();
};

#endif // MUSICCORE_H
