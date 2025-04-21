#ifndef GET_H
#define GET_H

#include "core.h"
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
#include "base/table.h"
#include "base/mediadata.h"
#include <QJsonObject>
#include <QJsonArray>

class Get : public QObject, virtual public Core
{
    Q_OBJECT

public:
    Q_INVOKABLE QStringList getArtistKeyList();
    Q_INVOKABLE QList<int> getArtist(QString key);
    Artist *getArtist(int id);
    QHash<int, Artist *> getArtist(QList<int> idList);
    QList<int> getArtistMusicList(int id);

    Q_INVOKABLE QStringList getAlbumKeyList();
    Q_INVOKABLE QList<int> getAlbum(QString key);
    Album *getAlbum(int id);
    QHash<int, Album *> getAlbum(QList<int> idList);
    QList<int> getAlbumMusicList(int id);

    Music *getMusic(int id);
    QHash<int, Music *> getMusic(QList<int> idList);
    QString getMusicUrl(int id);

    Q_INVOKABLE QString getAllList();
    Table *getList(int id);
    QList<int> getTableMusicList(int id);

    // 获得随机列表
    Q_INVOKABLE QList<int> getAlbumRandList();
    Q_INVOKABLE QList<int> getArtistRandList();
    Q_INVOKABLE QList<int> getMusicRandList();
    // 得到最新加入列表
    Q_INVOKABLE QList<int> getNewMusicList();
    // 得到最多播放音乐
    Q_INVOKABLE QList<int> getReadMoreList();

    QList<int> getIntList(const char *sql);
    MediaData getMediaFromStmt(sqlite3_stmt *stmt);

};

#endif // GET_H
