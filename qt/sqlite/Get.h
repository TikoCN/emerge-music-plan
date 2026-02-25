#ifndef GET_H
#define GET_H

#include "baseclass/Music.h"
#include "baseclass/Artist.h"
#include "baseclass/Playlist.h"
#include "baseclass/MediaData.h"
#include "baseclass/Album.h"
#include "Delete.h"
#include <QJsonArray>

class Get : public Delete
{
    Q_OBJECT
public:
    explicit Get(TLog *log) : Delete(log) {};

    // 歌手
    Q_INVOKABLE QStringList getArtistKeys();
    Q_INVOKABLE QList<int> getArtistByKey(const QString& key, int size, int start);
    ArtistPtr getArtist(int id);
    QHash<int, ArtistPtr> getArtist(const QList<int>& idList);
    Q_INVOKABLE [[nodiscard]] QList<int> getArtistMusic(int id, int size, int start, int sort);
    [[nodiscard]] QList<int> getArtistMusicAll(int id, int sort);
    int getArtistMusicFirst(int artistId);

    // 专辑
    Q_INVOKABLE QStringList getAlbumKeys();
    Q_INVOKABLE QList<int> getAlbumByKey(const QString& key, int size, int start);
    AlbumPtr getAlbum(int id);
    QHash<int, AlbumPtr> getAlbum(const QList<int>& idList);
    Q_INVOKABLE [[nodiscard]] QList<int> getAlbumMusic(int id, int size, int start, int sort);
    [[nodiscard]] QList<int> getAlbumMusicAll(int id, int sort);
    int getAlbumMusicFirst(int albumId);

    // 音乐
    Q_INVOKABLE QStringList getMusicKeys();
    Q_INVOKABLE QList<int> getMusicByKey(const QString& key, int size, int start);
    MusicPtr getMusic(int id);
    QHash<int, MusicPtr> getMusic(const QList<int>& idList);
    QString getMusicUrl(int id);

    // 播放列表
    Q_INVOKABLE QString getAllList();
    PlayListPtr getList(int id);
    Q_INVOKABLE [[nodiscard]] QList<int> getPlayListMusic(int id, int size, int start, int sort);
    [[nodiscard]] QList<int> getPlayListMusicAll(int id, int sort);
    int getPlayListMusicFirst(int playListId);

    // 正在播放列表
    Q_INVOKABLE QList<int> getPlayingListMusic();

    // 获得随机列表
    Q_INVOKABLE QList<int> getAlbumRandList();
    Q_INVOKABLE QList<int> getArtistRandList();
    Q_INVOKABLE QList<int> getMusicRandList(int length = -1);
    // 得到最新加入列表
    Q_INVOKABLE QList<int> getNewMusicList();
    // 得到最多播放音乐
    Q_INVOKABLE QList<int> getReadMoreList();

    QList<int> getIntList(const QString &sql);
    static MediaData getMediaFromStmt(sqlite3_stmt *stmt);

    Q_INVOKABLE int checkArtistName(const QString& name);
    Q_INVOKABLE int checkAlbumName(const QString& name);
    Q_INVOKABLE int checkPlayListName(const QString& name);

    QStringList getAlbumNameList(int size, int start);
    QStringList getArtistNameList(int size, int start);

    [[nodiscard]] static QString getSelectMusicSortSql(int sort, const QString& masterTable, const QString &masterColumn, bool isLimit = true);
};

#endif // GET_H
