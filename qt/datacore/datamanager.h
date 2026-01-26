#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QMutex>
#include "baseclass/music.h"
#include "baseclass/playlist.h"
#include "baseclass/album.h"
#include "baseclass/artist.h"
#include "sqlite/sqlite.h"
#include <QJsonObject>

class DataManager : public QObject
{
    Q_OBJECT

public:
    SQLite *sql;
    TLog *log;

    enum CORE_TYPE {
        MUSIC,
        PLAYLIST,
        ARTIST,
        ALBUM
    };

    DataManager();

    AlbumPtr getAlbumCore(int id);
    Q_INVOKABLE QJsonObject getAlbumJson(int id);

    ArtistPtr getArtistCore(int id);
    Q_INVOKABLE QJsonObject getArtistJson(int id);

    MusicPtr getMusicCore(int id);
    QList<MusicPtr> getMusicCoreList(const QList<int>& idList);
    Q_INVOKABLE QJsonObject getMusicJson(int id);

    PlayListPtr getPlayListCore(int id);
    Q_INVOKABLE QJsonObject getPlayListJson(int id);

    void releaseAlbum(int id);
    void releaseArtist(int id);
    void releaseMusic(int id);
    void releasePlayList(int id);

private:

    void deleteOutCache(CORE_TYPE type, int id);

    QHash<int, AlbumPtr> m_albumHash;      // 专辑列表
    QHash<int, ArtistPtr> m_artistHash;     // 歌手列表
    QHash<int, MusicPtr> m_musicHash;       // 音乐核心列表
    QHash<int, PlayListPtr> m_playlistHash;       // 播放列表
    QMutex m_albumMutex;
    QMutex m_artistMutex;
    QMutex m_musicMutex;
    QMutex m_playlistMutex;

    QList<QPair<CORE_TYPE, int>> m_deleteList;
    QMutex m_deleteListMutex;
};

#endif // DATAMANAGER_H
