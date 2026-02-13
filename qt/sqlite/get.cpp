#include "get.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <qscreen_platform.h>

#include "baseclass/dataexception.h"

QStringList Get::getArtistKeys()
{
    QStringList keyList;
    try {
        const auto sql = "SELECT DISTINCT key FROM artist ORDER BY key ASC";

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &keyList);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> Get::getArtistByKey(const QString& key, int size, int start)
{
    QList<int> artistList;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT artist_id "
                          "FROM artist "
                          "WHERE key = ? "
                         "LIMIT ? OFFSET ?";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            artistList.append(id);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        artistList.clear();
    }

    stmtFree(stmt);
    return artistList;
}

ArtistPtr Get::getArtist(const int id)
{
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, ArtistPtr> hash = getArtist(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, ArtistPtr> Get::getArtist(const QList<int>& idList)
{
    QHash<int, ArtistPtr> artistHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT artist.name, artist.artist_id, artist.key, "
                          "GROUP_CONCAT(artist_music.music_id) AS artist_music, SUM(music.duration) "
                          "FROM artist "
                          "JOIN artist_music ON artist.artist_id = artist_music.artist_id "
                          "JOIN music ON artist_music.music_id = music.music_id "
                          "WHERE artist.artist_id = ? LIMIT 1";

        stmtPrepare(&stmt, sql);
        for (int i : idList) {
            stmtPrepare(&stmt, sql);
            stmtBindInt(stmt, 1, i);
            stmtStep(stmt);
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            const int id = sqlite3_column_int(stmt, 1);
            const QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 3)).split(",");
            const long long duration = sqlite3_column_int64(stmt, 4);

            ArtistPtr artist(new Artist(name, id, key));
            artist->duration = duration;
            for (const auto & j : list) {
                artist->musicList.append(j.toInt());
            }
            artistHash.insert(i, artist);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        artistHash.clear();
    }

    stmtFree(stmt);
    return artistHash;
}

QList<int> Get::getArtistMusicList(const int id)
{
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT music_id "
                          "FROM artist_music "
                          "WHERE artist_id = ?";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        while (stmtStep(stmt)) {
            const int aim = sqlite3_column_int(stmt, 0);
            list.append(aim);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QStringList Get::getAlbumKeys()
{
    QStringList keyList;
    try {
        const auto sql = "SELECT DISTINCT key FROM album ORDER BY key ASC";

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &keyList);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> Get::getAlbumByKey(const QString& key, int size, int start)
{
    QList<int> albumList;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT album_id "
                          "FROM album "
                          "WHERE key = ? "
                          "LIMIT ? OFFSET ?";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            albumList.append(id);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        albumList.clear();
    }
    stmtFree(stmt);
    return albumList;
}

AlbumPtr Get::getAlbum(const int id)
{
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, AlbumPtr> hash = getAlbum(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, AlbumPtr> Get::getAlbum(const QList<int>& idList)
{
    QHash<int, AlbumPtr> albumHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT album.name, album.album_id, album.key, "
                          "GROUP_CONCAT(music.music_id) AS album_music, SUM(music.duration) "
                          "FROM album "
                          "JOIN music ON album.album_id = music.album_id "
                          "WHERE album.album_id = ? LIMIT 1";

        stmtPrepare(&stmt, sql);
        for (int i : idList) {
            stmtPrepare(&stmt, sql);
            stmtBindInt(stmt, 1, i);
            stmtStep(stmt);
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            const int id = sqlite3_column_int(stmt, 1);
            const QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 3)).split(",");
            const long long duration = sqlite3_column_int64(stmt, 4);

            AlbumPtr album(new Album(name, id, key));
            album->duration = duration;
            for (const auto & j : list) {
                album->musicList.append(j.toInt());
            }
            albumHash.insert(i, album);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        albumHash.clear();
    }
    stmtFree(stmt);
    return albumHash;
}

QList<int> Get::getAlbumMusicList(const int id)
{
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT music_id "
                          "FROM music "
                          "WHERE album_id = ?";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        while (stmtStep(stmt)) {
            const int aim = sqlite3_column_int(stmt, 0);
            list.append(aim);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QStringList Get::getMusicKeys() {
    QStringList keyList;
    try {
        const auto sql = "SELECT DISTINCT key FROM music ORDER BY key ASC";

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &keyList);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> Get::getMusicIdByKey(const QString &key, const int size, const int start) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT music_id "
                          "FROM music "
                          "WHERE key = ? "
                          "LIMIT ? OFFSET ?";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            list.append(id);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QString Get::getMusicUrl(const int id)
{
    sqlite3_stmt *stmt = nullptr;
    QString url;
    try {
        const auto sql = "SELECT url FROM music WHERE music_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        url = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return url;
}

MusicPtr Get::getMusic(const int id)
{
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, MusicPtr> hash = getMusic(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, MusicPtr> Get::getMusic(const QList<int>& idList)
{
    QHash<int, MusicPtr> hash;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "SELECT music.music_id, music.title, music.duration, music.insert_time, "
                          "music.level, music.love, music.play_number, music.url, "
                          "album.name, GROUP_CONCAT(artist.name) AS artist_names, music.album_id "
                          "FROM music "
                          "JOIN album ON album.album_id = music.album_id "
                          "JOIN artist_music ON artist_music.music_id = music.music_id "
                          "JOIN artist ON artist_music.artist_id = artist.artist_id "
                          "WHERE music.music_id = ? "
                          "GROUP BY music.music_id";
        stmtPrepare(&stmt, sql);
        for (int i : idList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, i);
            stmtStep(stmt);
            MusicPtr music(new Music);

            music->id = sqlite3_column_int(stmt, 0);
            music->title = QString::fromUtf8(sqlite3_column_text(stmt, 1));
            music->duration = sqlite3_column_int64(stmt, 2);
            music->insetTime = sqlite3_column_int64(stmt, 3);
            music->level = sqlite3_column_int(stmt, 4);
            music->isLove = sqlite3_column_int(stmt, 5) == 1;
            music->playNumber = sqlite3_column_int(stmt, 6);
            music->url = QString::fromUtf8(sqlite3_column_text(stmt, 7));
            music->album = QString::fromUtf8(sqlite3_column_text(stmt, 8));
            music->artistList = QString::fromUtf8(sqlite3_column_text(stmt, 9)).split(",");
            music->albumId = sqlite3_column_int(stmt, 10);

            music->fromFileInfo(QFileInfo(music->url));
            hash.insert(i, music);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        hash.clear();
    }
    stmtFree(stmt);
    return hash;
}

QString Get::getAllList()
{
    QJsonArray array;
    try {
        const auto sql = "SELECT list_id, name, is_dir, url FROM playlist";

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)->int{
            if (data == nullptr) {
                return SQLITE_ERROR;
            }

            if (!QFile::exists(QString(argv[3])))
                return SQLITE_OK;

            auto *jsons = static_cast<QJsonArray *>(data);
            QJsonObject obj;
            obj.insert("playlistId", QString(argv[0]).toInt());
            obj.insert("name", QString(argv[1]));
            obj.insert("isDir", QString(argv[2]).toInt());
            jsons->append(obj);
            return SQLITE_OK;
        };

        sqlExec(sql, callback, &array);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return "";
    }

    const QJsonDocument doc(array);
    return doc.toJson();
}

PlayListPtr Get::getList(const int id)
{
    PlayListPtr playlist(new PlayList);
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "SELECT l.list_id, l.name, l.sort, l.url, l.is_dir, SUM(m.duration) AS total_duration, GROUP_CONCAT(lm.music_id) AS music_ids "
                          "FROM playlist as l "
                          "JOIN playlist_music as lm ON l.list_id = lm.list_id "
                          "JOIN music as m ON lm.music_id = m.music_id "
                          "WHERE l.list_id = ? "
                          "LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        playlist->id = sqlite3_column_int(stmt, 0);
        playlist->name = QString::fromUtf8(sqlite3_column_text(stmt, 1));
        playlist->sort = static_cast<PlayList::SORT_TYPE>(sqlite3_column_int(stmt, 2));
        playlist->url  = QString::fromUtf8(sqlite3_column_text(stmt, 3));
        playlist->isDir = sqlite3_column_int(stmt, 4) == 1;
        playlist->duration = sqlite3_column_int64(stmt, 5);

        for (QStringList list = QString::fromUtf8(sqlite3_column_text(stmt, 6)).split(","); const QString & i : list) {
            playlist->musicList.append(i.toInt());
        }

    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return playlist;
}

QList<int> Get::getPlayListMusicList(const int id)
{
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "SELECT music_id "
                          "FROM playlist_music "
                          "WHERE list_id = ?";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        while (stmtStep(stmt)) {
            const int aim = sqlite3_column_int(stmt, 0);
            list.append(aim);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QList<int> Get::getAlbumRandList()
{
    const auto sql = "SELECT album_id FROM album ORDER BY RANDOM() LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getArtistRandList()
{
    const auto sql = "SELECT artist_id FROM artist ORDER BY RANDOM() LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getMusicRandList(int length)
{
    length = (length == -1) ? 15 : length;

    const QString sql = QString("SELECT music_id FROM music ORDER BY RANDOM() LIMIT %1").arg(length);
    return getIntList(sql.toStdString().c_str());
}

QList<int> Get::getNewMusicList()
{
    const auto sql = "SELECT music_id FROM music ORDER BY play_number DESC LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getReadMoreList()
{
    const auto sql = "SELECT music_id FROM music ORDER BY insert_time DESC LIMIT 15";
    return getIntList(sql);
}

QList<int> Get::getIntList(const char *sql)
{
    QList<int> idList;
    try {
        sqlExec(sql, idListCallBack, &idList);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return {};
    }
    return idList;
}

MediaData Get::getMediaFromStmt(sqlite3_stmt *stmt)
{
    /*
     * "SELECT music.title, music.duration, "
     * "music.level, music.love, music.play_number, music.url, "
     * "album.name, GROUP_CONCAT(artist.name) AS artist_names "
     */

    MediaData data;
    data.title = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    data.duration = sqlite3_column_int64(stmt, 1);
    data.level = sqlite3_column_int(stmt, 2);
    data.isLove = sqlite3_column_int(stmt, 3) == 1;
    data.playNumber = sqlite3_column_int(stmt, 4);
    data.url = QString::fromUtf8(sqlite3_column_text(stmt, 5));
    data.album= QString::fromUtf8(sqlite3_column_text(stmt, 6));
    data.artistList = QString::fromUtf8(sqlite3_column_text(stmt, 7)).split(",");
    return data;
}

int Get::checkArtistName(const QString& name)
{
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        const auto sql = "SELECT COALESCE("
                          "(SELECT artist_id FROM artist WHERE name = ? LIMIT 1), "
                          "-1) AS artist_id";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return r;
}

int Get::checkAlbumName(const QString& name)
{
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        const auto sql = "SELECT COALESCE("
                          "(SELECT album_id FROM album WHERE name = ? LIMIT 1), "
                          "-1) AS album_id";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return r;
}

int Get::checkPlayListName(const QString& name)
{
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        const auto sql = "SELECT COALESCE("
                          "(SELECT list_id FROM playlist WHERE name = ? LIMIT 1), "
                          "-1) AS list_id";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return r;
}

QStringList Get::getAlbumNameList(const int size, const int start) {
    QStringList albumNameList;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "SELECT name "
                         "FROM album "
                         "ORDER by name "
                         "LIMIT ? OFFSET ?";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, size);
        stmtBindInt(stmt, 2, start);
        while (stmtStep(stmt)) {
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            albumNameList.append(name);
        };

    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }
    
    stmtFree(stmt);
    return albumNameList;
}

QStringList Get::getArtistNameList(const int size, const int start) {
    QStringList artistNameList;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "SELECT name "
                         "FROM artist "
                         "ORDER by name "
                         "LIMIT ? OFFSET ?";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, size);
        stmtBindInt(stmt, 2, start);
        while (stmtStep(stmt)) {
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            artistNameList.append(name);
        };

    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }
    
    stmtFree(stmt);
    return artistNameList;
}

