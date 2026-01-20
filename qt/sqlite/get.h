#ifndef GET_H
#define GET_H

#include "baseclass/music.h"
#include "baseclass/album.h"
#include "baseclass/artist.h"
#include "baseclass/playlist.h"
#include "baseclass/mediadata.h"
#include "update.h"
#include <QJsonObject>
#include <QJsonArray>

class Get : public Update
{
    Q_OBJECT
public:
    Q_INVOKABLE QStringList getArtistKeyList();
    Q_INVOKABLE QList<int> getArtist(QString key);

    // 歌手
    ArtistPtr getArtist(int id);
    QHash<int, ArtistPtr> getArtist(QList<int> idList);
    QList<int> getArtistMusicList(int id);

    Q_INVOKABLE QStringList getAlbumKeyList();
    Q_INVOKABLE QList<int> getAlbum(QString key);

    // 专辑
    AlbumPtr getAlbum(int id);
    QHash<int, AlbumPtr> getAlbum(QList<int> idList);
    QList<int> getAlbumMusicList(int id);

    // 音乐
    MusicPtr getMusic(int id);
    QHash<int, MusicPtr> getMusic(QList<int> idList);
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
    MediaData getMediaFromStmt(sqlite3_stmt *stmt);

    Q_INVOKABLE int checkArtistName(QString name);
    Q_INVOKABLE int checkAlbumName(QString name);
    Q_INVOKABLE int checkPlayListName(QString name);

    int getArtistSize();
    int getAlbumSize();
};

#endif // GET_H
