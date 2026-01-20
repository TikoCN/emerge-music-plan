#ifndef MUSICCORE_H
#define MUSICCORE_H

#include <QObject>
#include <QJsonObject>
#include <QMutex>
#include "base/music.h"
#include "base/playlist.h"
#include "base/album.h"
#include "base/artist.h"

class MusicCore : public QObject
{
    Q_OBJECT

public:
    static MusicCore* getInstance() {
        return instance;
    }

    static void buildInstance() {
        if (instance == nullptr) {
            instance = new MusicCore;
        }
    }

    static void freeInstance() {
        if (instance != nullptr) {
            delete instance;
        }
    }

    enum CORE_TYPE {
        MUSIC,
        PLAYLIST,
        ARTIST,
        ALBUM
    };

<<<<<<< Updated upstream:musiccore.h
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
=======
    AlbumPtr getAlbumCore(int id);
    Q_INVOKABLE QJsonObject getAlbumJson(int id);
    ArtistPtr getArtistCore(int id);
    Q_INVOKABLE QJsonObject getArtistJson(int id);
    MusicPtr getMusicCore(int id);
    QList<MusicPtr> getMusicCoreList(QList<int> idList);
    Q_INVOKABLE QJsonObject getMusicJson(int id);
    PlayListPtr getPlayListCore(int id);
    Q_INVOKABLE QJsonObject getPlayListJson(int id);

    QString getAlbumName(int id);
    QString getArtistName(int id);

    Q_INVOKABLE QList<int> playListShowAllMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowLoveMusic(int playListId);
    Q_INVOKABLE QList<int> playListShowSearchMusic(int playListId, QString e);
    Q_INVOKABLE QList<int> playListShowSortMusic(int playListId, PlayList::SORT_TYPE sort);

    Q_INVOKABLE void appendPlayList(QString name);
    Q_INVOKABLE void clearNullItem();
    Q_INVOKABLE void loadMusic(QList<int> idList);
    Q_INVOKABLE void playlistAppendMusic(int id, QList<int> musicList);

    void releaseAlbum(int id);
    void releaseArtist(int id);
    void releaseMusic(int id);
    void releasePlayList(int id);
>>>>>>> Stashed changes:qt/musiccore.h

signals:
    void buildAlbumPlayer();
    void buildArtistPlayer();
    void buildPlayListPlayer();
    void finish();
<<<<<<< Updated upstream:musiccore.h
=======

private:
    explicit MusicCore();
    ~MusicCore();

    void deleteOutChace(MusicCore::CORE_TYPE type, int id);

    QHash<int, AlbumPtr> m_albumHash;      // 专辑列表
    QHash<int, ArtistPtr> m_artistHash;     // 歌手列表
    QHash<int, MusicPtr> m_musicHash;       // 音乐核心列表
    QHash<int, PlayListPtr> m_playlistHash;       // 播放列表
    QMutex m_albumMutex;
    QMutex m_artistMutex;
    QMutex m_musicMutex;
    QMutex m_playlistMutex;

    QList<QPair<MusicCore::CORE_TYPE, int>> m_deleteList;
    QMutex m_deleteListMutex;

    static MusicCore *instance;
>>>>>>> Stashed changes:qt/musiccore.h
};

#endif // MUSICCORE_H
