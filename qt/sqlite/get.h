#ifndef GET_H
#define GET_H

#include "baseclass/music.h"
#include "baseclass/album.h"
#include "baseclass/artist.h"
#include "baseclass/playlist.h"
#include "baseclass/mediadata.h"
#include "update.h"
#include <QJsonArray>

class Get : public Update
{
    Q_OBJECT
public:
    explicit Get(TLog *log) : Update(log) {};

    // 歌手
    Q_INVOKABLE QStringList getArtistKeys();
    Q_INVOKABLE QList<int> getArtistByKey(const QString& key, int size, int start);
    ArtistPtr getArtist(int id);
    QHash<int, ArtistPtr> getArtist(const QList<int>& idList);
    QList<int> getArtistMusicList(int id);

    // 专辑
    Q_INVOKABLE QStringList getAlbumKeys();
    Q_INVOKABLE QList<int> getAlbumByKey(const QString& key, int size, int start);
    AlbumPtr getAlbum(int id);
    QHash<int, AlbumPtr> getAlbum(const QList<int>& idList);
    QList<int> getAlbumMusicList(int id);

    // 音乐
    Q_INVOKABLE QStringList getMusicKeys();
    Q_INVOKABLE QList<int> getMusicIdByKey(const QString& key, int size, int start);
    MusicPtr getMusic(int id);
    QHash<int, MusicPtr> getMusic(const QList<int>& idList);
    QString getMusicUrl(int id);

    // 播放列表
    Q_INVOKABLE QString getAllList();
    PlayListPtr getList(int id);
    QList<int> getPlayListMusicList(int id);

    // 获得随机列表
    Q_INVOKABLE QList<int> getAlbumRandList();
    Q_INVOKABLE QList<int> getArtistRandList();
    Q_INVOKABLE QList<int> getMusicRandList(int length = -1);
    // 得到最新加入列表
    Q_INVOKABLE QList<int> getNewMusicList();
    // 得到最多播放音乐
    Q_INVOKABLE QList<int> getReadMoreList();

    QList<int> getIntList(const char *sql);
    static MediaData getMediaFromStmt(sqlite3_stmt *stmt);

    Q_INVOKABLE int checkArtistName(const QString& name);
    Q_INVOKABLE int checkAlbumName(const QString& name);
    Q_INVOKABLE int checkPlayListName(const QString& name);

    QStringList getAlbumNameList(int size, int start);
    QStringList getArtistNameList(int size, int start);
};

#endif // GET_H
