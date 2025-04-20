#include "get.h"
#include <QJsonDocument>

QStringList Get::getArtistKeyList()
{
    QStringList keyList;
    try {
        const char *sql = "SELECT key FROM artist";

        sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            QStringList *keyList = static_cast<QStringList *>(data);
            keyList->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &keyList);
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

QList<int> Get::getArtist(QString key)
{
    QList<int> artistList;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT artist_id "
                          "FROM artist "
                          "WHERE key = ? ";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        while (stmtStep(stmt)) {
            int id = sqlite3_column_int(stmt, 0);
            artistList.append(id);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        artistList.clear();
    }
    sqlite3_finalize(stmt);
    return artistList;
}

Artist *Get::getArtist(int id)
{
    QList<int> idList;
    idList.append(id);
    QHash<int, Artist *> hash = getArtist(idList);

    if (hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, Artist *> Get::getArtist(QList<int> idList)
{
    QHash<int, Artist *> artistHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT artist.name, artist.artist_id, artist.key, "
                          "GROUP_CONCAT(artist_music.music_id) AS artist_music "
                          "FROM artist "
                          "JOIN artist_music ON artist.artist_id = artist_music.artist_id "
                          "WHERE artist.artist_id = ? LIMIT 1";

        stmtPrepare(&stmt, sql);
        for (int i = 0; i < idList.size(); ++i) {
            stmtPrepare(&stmt, sql);
            stmtBindInt(stmt, 1, idList[i]);
            stmtStep(stmt);
            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 3)).split(",");

            Artist *artist = new Artist(name, id, key);
            for (int j = 0; j < list.size(); ++j) {
                artist->musicList.append(list[j].toInt());
            }
            artistHash.insert(idList[i], artist);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        QList<Artist *> list = artistHash.values();
        while (!list.isEmpty()) {
            list.takeLast()->deleteLater();
        }artistHash.clear();
    }
    sqlite3_finalize(stmt);
    return artistHash;
}

QStringList Get::getAlbumKeyList()
{
    QStringList keyList;
    try {
        const char *sql = "SELECT key FROM album";

        sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            QStringList *keyList = static_cast<QStringList *>(data);
            keyList->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &keyList);
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

QList<int> Get::getAlbum(QString key)
{
    QList<int> albumList;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT album_id "
                          "FROM album "
                          "WHERE key = ? ";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        while (stmtStep(stmt)) {
            int id = sqlite3_column_int(stmt, 0);
            albumList.append(id);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        albumList.clear();
    }
    sqlite3_finalize(stmt);
    return albumList;
}

Album *Get::getAlbum(int id)
{
    QList<int> idList;
    idList.append(id);
    QHash<int, Album *> hash = getAlbum(idList);

    if (hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, Album *> Get::getAlbum(QList<int> idList)
{
    QHash<int, Album *> albumHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT album.name, album.album_id, album.key, "
                          "GROUP_CONCAT(music.music_id) AS album_music "
                          "FROM album "
                          "JOIN music ON album.album_id = music.album_id "
                          "WHERE album.album_id = ? LIMIT 1";

        stmtPrepare(&stmt, sql);
        for (int i = 0; i < idList.size(); ++i) {
            stmtPrepare(&stmt, sql);
            stmtBindInt(stmt, 1, idList[i]);
            stmtStep(stmt);
            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 3)).split(",");

            Album *album = new Album(name, id, key);
            for (int j = 0; j < list.size(); ++j) {
                album->musicList.append(list[j].toInt());
            }
            albumHash.insert(idList[i], album);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        QList<Album *> list = albumHash.values();
        while (!list.isEmpty()) {
            list.takeLast()->deleteLater();
        }albumHash.clear();
    }
    sqlite3_finalize(stmt);
    return albumHash;
}

QString Get::getMusicUrl(int id)
{
    sqlite3_stmt *stmt = nullptr;
    QString url;
    try {
        const char *sql = "SELECT url FROM music WHERE music_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        url = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    } catch (QString e) {
    }
    sqlite3_finalize(stmt);
    return url;
}

Music *Get::getMusic(int id)
{
    QList<int> idList;
    idList.append(id);
    QHash<int, Music*> hash = getMusic(idList);

    if (hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, Music *> Get::getMusic(QList<int> idList)
{
    QHash<int, Music *> hash;
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "SELECT music.music_id, music.title, music.duration, music.insert_time, "
                          "music.level, music.love, music.play_number, music.url, "
                          "album.name, GROUP_CONCAT(artist.name) AS artist_names "
                          "FROM music "
                          "JOIN album ON album.album_id = music.album_id "
                          "JOIN artist_music ON artist_music.music_id = music.music_id "
                          "JOIN artist ON artist_music.artist_id = artist.artist_id "
                          "WHERE music.music_id = ? "
                          "GROUP BY music.music_id";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < idList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, idList[i]);
            stmtStep(stmt);
            Music *music = new Music;
            MediaData data = getMediaFromStmt(stmt);
            music->setMedia(data);
            music->fromFileInfo(QFileInfo(music->url));
            hash.insert(idList[i], music);
        }
    } catch (QString e) {
        QList<Music *> list = hash.values();
        while (!list.isEmpty()) {
            list.takeLast()->deleteLater();
        }
        hash.clear();
    }
    return hash;
}

QString Get::getAllList()
{
    QJsonArray array;
    try {
        const char *sql = "SELECT list_id, name FROM playlist";

        sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            QJsonArray *array = static_cast<QJsonArray *>(data);
            QJsonObject obj;
            obj.insert("tableId", QString(argv[0]));
            obj.insert("name", QString(argv[1]));
            array->append(obj);
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &array);
    } catch (QString e) {
        return " ";
    }
    QJsonDocument doc(array);
    return QString(doc.toJson());
}

Table *Get::getList(int id)
{
    Table *table = nullptr;
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "SELECT l.list_id, l.name, l.sort, l.url, l.is_dir, GROUP_CONCAT(lm.music_id) AS music_ids "
                          "FROM playlist as l "
                          "JOIN playlist_music as lm ON l.list_id = lm.list_id "
                          "WHERE l.list_id = ? "
                          "LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        table = new Table;
        table->tableId = sqlite3_column_int(stmt, 0);
        table->name = QString::fromUtf8(sqlite3_column_text(stmt, 1));
        table->sort = (Table::SORT_TYPE)sqlite3_column_int(stmt, 2);
        table->url  = QString::fromUtf8(sqlite3_column_text(stmt, 3));
        table->isDir = sqlite3_column_int(stmt, 4) == 1;
        QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 5)).split(",");

        for (int i = 0; i < list.size(); ++i) {
            table->musicList.append(list[i].toInt());
        }

    } catch (QString e) {
        delete table;
        table = nullptr;
    }
    return table;
}

QList<int> Get::getAlbumRandList()
{
    const char *sql = "SELECT album_id FROM album ORDER BY RANDOM() LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getArtistRandList()
{
    const char *sql = "SELECT artist_id FROM artist ORDER BY RANDOM() LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getMusicRandList()
{
    const char *sql = "SELECT music_id FROM music ORDER BY RANDOM() LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getNewMusicList()
{
    const char *sql = "SELECT music_id FROM music ORDER BY play_number DESC LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getReadMoreList()
{
    const char *sql = "SELECT music_id FROM music ORDER BY insert_time DESC LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getIntList(const char *sql)
{
    QList<int> idList;
    try {
        sqlExec(sql, idListCallBack, &idList);
    } catch (QString e) {
        return QList<int>();
    }
    return idList;
}

MediaData Get::getMediaFromStmt(sqlite3_stmt *stmt)
{
    /*
     * "SELECT music.music_id, music.title, music.duration, music.insert_time, "
     * "music.level, music.love, music.play_number, music.url, "
     * "album.name, GROUP_CONCAT(artist.name) AS artist_names "
     */

    MediaData data;
    data.id = sqlite3_column_int(stmt, 0);
    data.title = QString::fromUtf8(sqlite3_column_text(stmt, 1));
    data.duration = sqlite3_column_int64(stmt, 2);
    data.insetTime = sqlite3_column_int64(stmt, 3);
    data.level = sqlite3_column_int(stmt, 4);
    data.isLove = sqlite3_column_int(stmt, 5) == 1;
    data.playNumber = sqlite3_column_int(stmt, 6);
    data.url = QString::fromUtf8(sqlite3_column_text(stmt, 7));
    data.album= QString::fromUtf8(sqlite3_column_text(stmt, 8));
    data.artistList = QString::fromUtf8(sqlite3_column_text(stmt, 9)).split(",");
    return data;
}

