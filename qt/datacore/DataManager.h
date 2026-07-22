#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QMutex>
#include "baseclass/Music.h"
#include "baseclass/Playlist.h"
#include "baseclass/Artist.h"
#include "baseclass/Album.h"
#include "sqlite/Sqlite.h"
#include <QJsonObject>

class DataManager : public QObject {
    Q_OBJECT

public:
    enum CORE_TYPE {
        MUSIC,
        PLAYLIST,
        ARTIST,
        ALBUM
    };

    DataManager();

    AlbumPtr    getAlbumCore(int id);
    QJsonObject getAlbumJson(int id);
    Album       getAlbumData(int id);

    ArtistPtr   getArtistCore(int id);
    QJsonObject getArtistJson(int id);
    Artist      getArtistData(int id);

    MusicPtr        getMusicCore(int id);
    QList<MusicPtr> getMusicCoreList(const QList<int> &idList);
    QJsonObject     getMusicJson(int id);
    Music           getMusicData(int id);

    PlayListPtr getPlayListCore(int id);
    QJsonObject getPlayListJson(int id);
    PlayList    getPlayListData(int id);

    void releaseAlbum(int id);
    void releaseArtist(int id);
    void releaseMusic(int id);
    void releasePlayList(int id);

private:
    void deleteOutCache(CORE_TYPE type, int id);

    QHash<int, AlbumPtr>    m_albumHash;    // 专辑列表
    QHash<int, ArtistPtr>   m_artistHash;   // 歌手列表
    QHash<int, MusicPtr>    m_musicHash;    // 音乐核心列表
    QHash<int, PlayListPtr> m_playlistHash; // 播放列表
    QMutex                  m_albumMutex;
    QMutex                  m_artistMutex;
    QMutex                  m_musicMutex;
    QMutex                  m_playlistMutex;

    QList<QPair<CORE_TYPE, int> > m_deleteList;
    QMutex                        m_deleteListMutex;
};

#endif // DATAMANAGER_H
