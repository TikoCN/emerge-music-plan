#ifndef GET_H
#define GET_H

#include "baseclass/Music.h"
#include "baseclass/Artist.h"
#include "baseclass/Playlist.h"
#include "baseclass/MediaData.h"
#include "baseclass/Album.h"
#include "Core.h"

class Get {
private:
    Core *core;

public:
    explicit Get(Core *core)
        : core(core) {
    };

    // 歌手
    [[nodiscard]] QStringList           getArtistKeys() const;
    [[nodiscard]] QList<int>            getArtistByKey(const QString &key, int size, int start) const;
    [[nodiscard]] ArtistPtr             getArtist(int id) const;
    [[nodiscard]] QHash<int, ArtistPtr> getArtist(const QList<int> &idList) const;
    [[nodiscard]] QList<int>            getArtistMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>            getArtistMusicAll(int id, int sort) const;
    [[nodiscard]] int                   getArtistMusicFirst(int artistId) const;

    // 专辑
    [[nodiscard]] QStringList          getAlbumKeys() const;
    [[nodiscard]] QList<int>           getAlbumByKey(const QString &key, int size, int start) const;
    [[nodiscard]] AlbumPtr             getAlbum(int id) const;
    [[nodiscard]] QHash<int, AlbumPtr> getAlbum(const QList<int> &idList) const;
    [[nodiscard]] QList<int>           getAlbumMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>           getAlbumMusicAll(int id, int sort) const;
    [[nodiscard]] int                  getAlbumMusicFirst(int albumId) const;

    // 音乐
    [[nodiscard]] QStringList          getMusicKeys() const;
    [[nodiscard]] QList<int>           getMusicByKey(const QString &key, int size, int start) const;
    [[nodiscard]] MusicPtr             getMusic(int id) const;
    [[nodiscard]] QHash<int, MusicPtr> getMusic(const QList<int> &idList) const;
    [[nodiscard]] QString              getMusicUrl(int id) const;

    // 播放列表
    [[nodiscard]] QString     getAllList() const;
    [[nodiscard]] PlayListPtr getList(int id) const;
    [[nodiscard]] QList<int>  getPlayListMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getPlayListMusicAll(int id, int sort) const;
    [[nodiscard]] int         getPlayListMusicFirst(int playListId) const;

    // 正在播放列表
    [[nodiscard]] QList<int> getPlayingListMusic() const;

    // 获得随机列表
    [[nodiscard]] QList<int> getAlbumRandList() const;
    [[nodiscard]] QList<int> getArtistRandList() const;
    [[nodiscard]] QList<int> getMusicRandList(int length = -1) const;
    // 得到最新加入列表
    [[nodiscard]] QList<int> getNewMusicList() const;
    // 得到最多播放音乐
    [[nodiscard]] QList<int> getReadMoreList() const;

    [[nodiscard]] QList<int> getIntList(const QString &sql) const;
    static MediaData         getMediaFromStmt(sqlite3_stmt *stmt);

    [[nodiscard]] int checkArtistName(const QString &name) const;
    [[nodiscard]] int checkAlbumName(const QString &name) const;
    [[nodiscard]] int checkPlayListName(const QString &name) const;

    [[nodiscard]] QStringList getAlbumNameList(int size, int start) const;
    [[nodiscard]] QStringList getArtistNameList(int size, int start) const;

    [[nodiscard]] static QString getSelectMusicSortSql(int sort, const QString &masterTable, const QString &masterColumn, bool isLimit = true);
};

#endif // GET_H
