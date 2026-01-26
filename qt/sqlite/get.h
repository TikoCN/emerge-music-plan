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
    Q_INVOKABLE QStringList getArtistKeyList();
    Q_INVOKABLE QList<int> getArtist(const QString& key);

    // 歌手
    ArtistPtr getArtist(int id);
    QHash<int, ArtistPtr> getArtist(const QList<int>& idList);
    QList<int> getArtistMusicList(int id);

    Q_INVOKABLE QStringList getAlbumKeyList();
    Q_INVOKABLE QList<int> getAlbum(const QString& key);

    // 专辑
    AlbumPtr getAlbum(int id);
    QHash<int, AlbumPtr> getAlbum(const QList<int>& idList);
    QList<int> getAlbumMusicList(int id);

    // 音乐
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

    int getArtistSize();
    int getAlbumSize();
};

#endif // GET_H
