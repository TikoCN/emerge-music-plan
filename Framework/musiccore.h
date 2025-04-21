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

    Q_INVOKABLE Table *getTable(int id);

    Q_INVOKABLE Album *getAlbum(int id);

    Q_INVOKABLE Artist *getArtist(int id);

    Q_INVOKABLE Music *getMusic(int id);
    Q_INVOKABLE QList<Music *> getMusic(QList<int > idList);
    Q_INVOKABLE void loadMusic(QList<int > idList);


    QList<int> selectSearchMusic(QList<int > idList, QString e);
    QList<int> sortMusic(QList<int > idList, Table::SORT_TYPE sort);
    QList<int> selectLoveMusic(QList<int > idList);

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
