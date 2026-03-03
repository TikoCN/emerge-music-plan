#include "Get.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <qscreen_platform.h>

#include "baseclass/DataException.h"

QStringList Get::getArtistKeys() {
    QStringList keyList;
    try {
        // SELECT DISTINCT key FROM artist ORDER BY key ASC
        const auto sql = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1 ASC")
                .arg(LiteralConstant::Column::NAME_KEY) // key 字段，请确认常量值是否为 "key"
                .arg(LiteralConstant::Table::ARTIST);

        const sqlite3_callback callback = [](void *data, int, char **argv, char **)-> int {
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExecuteCallBack(sql.toUtf8(), callback, &keyList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

/**
 * @brief 根据key得到对应 artist
 * @param key 名称关键词
 * @param size 页大小
 * @param start 初始未知
 * @return 返回的artist ID列表
 */
QList<int> Get::getArtistByKey(const QString &key, const int size, const int start) {
    QList<int> artistList;
    sqlite3_stmt *stmt = nullptr;

    try {
        // SELECT artist_id FROM artist WHERE key = ? LIMIT ? OFFSET ?
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                .arg(LiteralConstant::Column::ARTIST_ID)
                .arg(LiteralConstant::Table::ARTIST)
                .arg(LiteralConstant::Column::NAME_KEY); // key 字段
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            artistList.append(id);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        artistList.clear();
    }

    stmtFree(stmt);
    return artistList;
}


ArtistPtr Get::getArtist(const int id) {
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, ArtistPtr> hash = getArtist(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, ArtistPtr> Get::getArtist(const QList<int> &idList) {
    QHash<int, ArtistPtr> artistHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 复杂的 SELECT，注意别名和连接
        const auto sql = QString(
                    "SELECT %1.%2, %1.%3, %1.%4, %1.%5, "
                    "COUNT(DISTINCT %6.%7) AS music_count, SUM(%8.%9), "
                    "MIN(%8.%10) AS first_music_id "
                    "FROM %1 "
                    "JOIN %11 ON %1.%3 = %11.%12 "
                    "JOIN %8 ON %11.%7 = %8.%13 "
                    "WHERE %1.%3 = ? "
                    "GROUP BY %1.%3, %1.%2, %1.%4, %1.%5 "
                    "LIMIT 1")
                .arg(LiteralConstant::Table::ARTIST) // %1
                .arg(LiteralConstant::Column::ARTIST_NAME) // %2 (name)
                .arg(LiteralConstant::Column::ARTIST_ID) // %3 (artist_id)
                .arg(LiteralConstant::Column::NAME_KEY) // %4 (key)
                .arg(LiteralConstant::Column::SORT) // %5 (sort)
                .arg(LiteralConstant::Table::ARTIST_MUSIC) // %6 (artist_music)
                .arg(LiteralConstant::Column::MUSIC_ID) // %7 (music_id)
                .arg(LiteralConstant::Table::MUSIC) // %8 (music)
                .arg(LiteralConstant::Column::DURATION) // %9 (duration)
                .arg(LiteralConstant::Column::MUSIC_ID) // %10 (music_id)
                .arg(LiteralConstant::Table::ARTIST_MUSIC) // %11 (artist_music) 重复使用
                .arg(LiteralConstant::Column::ARTIST_ID) // %12 (artist_id)
                .arg(LiteralConstant::Column::MUSIC_ID); // %13 (music_id)

        stmtPrepare(&stmt, sql.toUtf8());
        for (int i: idList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, i);
            stmtStep(stmt);
            ArtistPtr artist(new Artist());

            artist->name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            artist->id = sqlite3_column_int(stmt, 1);
            artist->lineKey = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            artist->sortType = static_cast<SORT_TYPE>(sqlite3_column_int(stmt, 3));
            artist->musicCount = sqlite3_column_int(stmt, 4);
            artist->duration = sqlite3_column_int64(stmt, 5);
            artist->firstMusic = sqlite3_column_int(stmt, 6);

            artistHash.insert(i, artist);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        artistHash.clear();
    }

    stmtFree(stmt);
    return artistHash;
}

QList<int> Get::getArtistMusic(const int id, const int size, const int start, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::ARTIST_MUSIC,
                                               LiteralConstant::Column::ARTIST_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QList<int> Get::getArtistMusicAll(const int id, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::ARTIST_MUSIC,
                                               LiteralConstant::Column::ARTIST_ID, false);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QList<int> Get::getAlbumMusicAll(const int id, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;
    try {
        // SELECT music_id FROM album_music WHERE album_id = ?
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::ALBUM_MUSIC,
                                               LiteralConstant::Column::ALBUM_ID, false);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return list;
}

int Get::getArtistMusicFirst(const int artistId) {
    int musicId = -1;
    sqlite3_stmt *stmt = nullptr;
    try {
        // SELECT music_id FROM artist_music WHERE artist_id = ? LIMIT 1
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                .arg(LiteralConstant::Column::MUSIC_ID)
                .arg(LiteralConstant::Table::ARTIST_MUSIC)
                .arg(LiteralConstant::Column::ARTIST_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, artistId);
        stmtStep(stmt);
        musicId = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return musicId;
}

int Get::getAlbumMusicFirst(const int albumId) {
    int musicId = -1;
    sqlite3_stmt *stmt = nullptr;
    try {
        // SELECT music_id FROM album_music WHERE album_id = ? LIMIT 1
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                .arg(LiteralConstant::Column::MUSIC_ID)
                .arg(LiteralConstant::Table::ALBUM_MUSIC)
                .arg(LiteralConstant::Column::ALBUM_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, albumId);
        stmtStep(stmt);
        musicId = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return musicId;
}

int Get::getPlayListMusicFirst(int playListId) {
    int musicId = -1;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 注意：原 SQL 中表名为 "playList_music" 可能错误，根据常量为 "playlist_music"
        // 列名 playList_id 应为 playlist_id
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                .arg(LiteralConstant::Column::MUSIC_ID)
                .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                .arg(LiteralConstant::Column::PLAYLIST_ID); // 请确认常量值为 "playlist_id" 还是 "list_id"
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, playListId);
        stmtStep(stmt);
        musicId = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return musicId;
}

QList<int> Get::getPlayingListMusic() {
    const auto sql = QString("SELECT %1 FROM " + LiteralConstant::Table::PLAYINGLIST + " ORDER BY %2 ASC")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Column::POSITION);
    return getIntList(sql.toUtf8());
}

QStringList Get::getAlbumKeys() {
    QStringList keyList;
    try {
        // SELECT DISTINCT key FROM album ORDER BY key ASC
        const auto sql = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1 ASC")
                .arg(LiteralConstant::Column::NAME_KEY)
                .arg(LiteralConstant::Table::ALBUM);

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)-> int {
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExecuteCallBack(sql.toUtf8(), callback, &keyList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> Get::getAlbumByKey(const QString &key, int size, int start) {
    QList<int> albumList;
    sqlite3_stmt *stmt = nullptr;

    try {
        // SELECT album_id FROM album WHERE key = ? LIMIT ? OFFSET ?
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                .arg(LiteralConstant::Column::ALBUM_ID)
                .arg(LiteralConstant::Table::ALBUM)
                .arg(LiteralConstant::Column::NAME_KEY);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            albumList.append(id);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        albumList.clear();
    }
    stmtFree(stmt);
    return albumList;
}

AlbumPtr Get::getAlbum(const int id) {
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, AlbumPtr> hash = getAlbum(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, AlbumPtr> Get::getAlbum(const QList<int> &idList) {
    QHash<int, AlbumPtr> albumHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = QString(
                    "SELECT %1.%2, %1.%3, %1.%4, %1.%5, "
                    "COUNT(DISTINCT %6.%7) AS music_count, SUM(%8.%9), "
                    "MIN(%8.%10) AS first_music_id "
                    "FROM %1 "
                    "JOIN %11 ON %1.%3 = %11.%12 "
                    "JOIN %8 ON %11.%7 = %8.%13 "
                    "WHERE %1.%3 = ? "
                    "GROUP BY %1.%3, %1.%2, %1.%4, %1.%5 "
                    "LIMIT 1")
                .arg(LiteralConstant::Table::ALBUM) // %1
                .arg(LiteralConstant::Column::ALBUM_NAME) // %2 (name)
                .arg(LiteralConstant::Column::ALBUM_ID) // %3 (album_id)
                .arg(LiteralConstant::Column::NAME_KEY) // %4 (key)
                .arg(LiteralConstant::Column::SORT) // %5 (sort)
                .arg(LiteralConstant::Table::ALBUM_MUSIC) // %6 (album_music)
                .arg(LiteralConstant::Column::MUSIC_ID) // %7 (music_id)
                .arg(LiteralConstant::Table::MUSIC) // %8 (music)
                .arg(LiteralConstant::Column::DURATION) // %9 (duration)
                .arg(LiteralConstant::Column::MUSIC_ID) // %10 (music_id)
                .arg(LiteralConstant::Table::ALBUM_MUSIC) // %11 (album_music)
                .arg(LiteralConstant::Column::ALBUM_ID) // %12 (album_id)
                .arg(LiteralConstant::Column::MUSIC_ID); // %13 (music_id)

        stmtPrepare(&stmt, sql.toUtf8());
        for (int i: idList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, i);
            stmtStep(stmt);
            AlbumPtr album(new Album());

            album->name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            album->id = sqlite3_column_int(stmt, 1);
            album->lineKey = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            album->sortType = static_cast<SORT_TYPE>(sqlite3_column_int(stmt, 3));
            album->musicCount = sqlite3_column_int(stmt, 4);
            album->duration = sqlite3_column_int64(stmt, 5);
            album->firstMusic = sqlite3_column_int(stmt, 6);

            albumHash.insert(i, album);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        albumHash.clear();
    }
    stmtFree(stmt);
    return albumHash;
}

QList<int> Get::getAlbumMusic(const int id, const int size, const int start, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::ALBUM_MUSIC,
                                               LiteralConstant::Column::ALBUM_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return list;
}

QStringList Get::getMusicKeys() {
    QStringList keyList;
    try {
        // SELECT DISTINCT key FROM music ORDER BY key ASC
        const auto sql = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1 ASC")
                .arg(LiteralConstant::Column::NAME_KEY)
                .arg(LiteralConstant::Table::MUSIC);

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)-> int {
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        sqlExecuteCallBack(sql.toUtf8(), callback, &keyList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> Get::getMusicByKey(const QString &key, const int size, const int start) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        // SELECT music_id FROM music WHERE key = ? LIMIT ? OFFSET ?
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                .arg(LiteralConstant::Column::MUSIC_ID)
                .arg(LiteralConstant::Table::MUSIC)
                .arg(LiteralConstant::Column::NAME_KEY);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, key);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        while (stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            list.append(id);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QString Get::getMusicUrl(const int id) {
    sqlite3_stmt *stmt = nullptr;
    QString url;
    try {
        // SELECT url FROM music WHERE music_id=? LIMIT 1
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3=? LIMIT 1")
                .arg(LiteralConstant::Column::URL)
                .arg(LiteralConstant::Table::MUSIC)
                .arg(LiteralConstant::Column::MUSIC_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        url = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return url;
}

MusicPtr Get::getMusic(const int id) {
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, MusicPtr> hash = getMusic(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, MusicPtr> Get::getMusic(const QList<int> &idList) {
    QHash<int, MusicPtr> hash;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = QString(
                            "SELECT m.%1, m.%2, m.%3, m.%4, m.%5, m.%6, m.%7, m.%8, "
                            "GROUP_CONCAT(alb.%9) as album_names, GROUP_CONCAT(art.%10) AS artist_names "
                            "FROM " + LiteralConstant::Table::MUSIC + " m "
                            "JOIN " + LiteralConstant::Table::ALBUM_MUSIC + " alb_m ON alb_m.%1 = m.%1 "
                            "JOIN " + LiteralConstant::Table::ALBUM + " alb ON alb.%11 = alb_m.%11 "
                            "JOIN " + LiteralConstant::Table::ARTIST_MUSIC + " art_m ON art_m.%1 = m.%1 "
                            "JOIN " + LiteralConstant::Table::ARTIST + " art ON art.%12 = art_m.%12 "
                            "WHERE m.%1 = ? "
                            "GROUP BY m.%1")
                        .arg(LiteralConstant::Column::MUSIC_ID) // 1
                        .arg(LiteralConstant::Column::TITLE) // 2
                        .arg(LiteralConstant::Column::DURATION) // 3
                        .arg(LiteralConstant::Column::LAST_EDIT_TIME) // 4
                        .arg(LiteralConstant::Column::LEVEL) // 5
                        .arg(LiteralConstant::Column::IS_LOVE) // 6
                        .arg(LiteralConstant::Column::PLAY_NUMBER) // 7
                        .arg(LiteralConstant::Column::URL) // 8
                        .arg(LiteralConstant::Column::ALBUM_NAME) // 9
                        .arg(LiteralConstant::Column::ARTIST_NAME) // 10
                        .arg(LiteralConstant::Column::ALBUM_ID) // 11
                        .arg(LiteralConstant::Column::ARTIST_ID) // 12
                ;

        stmtPrepare(&stmt, sql.toUtf8());
        for (int i: idList) {
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
            music->artist = QString::fromUtf8(sqlite3_column_text(stmt, 9));

            music->fromFileInfo(QFileInfo(music->url));
            hash.insert(i, music);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        hash.clear();
    }
    stmtFree(stmt);
    return hash;
}

QString Get::getAllList() {
    QJsonArray array;
    try {
        // SELECT list_id, name, is_dir, url FROM playlist
        const auto sql = QString("SELECT %1, %2, %3, %4 FROM %5")
                .arg(LiteralConstant::Column::PLAYLIST_ID) // 请确认列名
                .arg(LiteralConstant::Column::PLAYLIST_NAME) // 请确认列名
                .arg(LiteralConstant::Column::IS_DIR)
                .arg(LiteralConstant::Column::URL)
                .arg(LiteralConstant::Table::PLAYLIST);

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)-> int {
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

        sqlExecuteCallBack(sql.toUtf8(), callback, &array);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        return "";
    }

    const QJsonDocument doc(array);
    return doc.toJson();
}

PlayListPtr Get::getList(const int id) {
    PlayListPtr playlist(new PlayList);
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = QString(
                    "SELECT l.%1, l.%2, l.%3, l.%4, l.%5, SUM(m.%6) AS total_duration, "
                    "COUNT(m.%7) as music_count, MIN(m.%7) AS first_music_id "
                    "FROM %8 as l "
                    "JOIN %9 as lm ON l.%1 = lm.%10 "
                    "JOIN %11 as m ON lm.%7 = m.%7 "
                    "WHERE l.%1 = ? "
                    "LIMIT 1")
                .arg(LiteralConstant::Column::PLAYLIST_ID) // %1 (list_id)
                .arg(LiteralConstant::Column::PLAYLIST_NAME) // %2 (name)
                .arg(LiteralConstant::Column::SORT) // %3 (sort)
                .arg(LiteralConstant::Column::URL) // %4 (url)
                .arg(LiteralConstant::Column::IS_DIR) // %5 (is_dir)
                .arg(LiteralConstant::Column::DURATION) // %6 (duration)
                .arg(LiteralConstant::Column::MUSIC_ID) // %7 (music_id)
                .arg(LiteralConstant::Table::PLAYLIST) // %8 (playlist)
                .arg(LiteralConstant::Table::PLAYLIST_MUSIC) // %9 (playlist_music)
                .arg(LiteralConstant::Column::PLAYLIST_ID) // %10 (list_id)
                .arg(LiteralConstant::Table::MUSIC); // %11 (music)

        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        playlist->id = sqlite3_column_int(stmt, 0);
        playlist->name = QString::fromUtf8(sqlite3_column_text(stmt, 1));
        playlist->sortType = static_cast<SORT_TYPE>(sqlite3_column_int(stmt, 2));
        playlist->url = QString::fromUtf8(sqlite3_column_text(stmt, 3));
        playlist->isDir = sqlite3_column_int(stmt, 4) == 1;
        playlist->duration = sqlite3_column_int64(stmt, 5);
        playlist->musicConut = sqlite3_column_int(stmt, 6);
        playlist->firstMusic = sqlite3_column_int(stmt, 7);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return playlist;
}

QList<int> Get::getPlayListMusic(const int id, const int size, const int start, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::PLAYLIST_MUSIC,
                                               LiteralConstant::Column::PLAYLIST_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtBindInt(stmt, 2, size);
        stmtBindInt(stmt, 3, start);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QList<int> Get::getPlayListMusicAll(const int id, const int sort) {
    QList<int> list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = getSelectMusicSortSql(sort,
                                               LiteralConstant::Table::PLAYLIST_MUSIC,
                                               LiteralConstant::Column::PLAYLIST_ID, false);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        list.clear();
    }
    stmtFree(stmt);
    return list;
}

QList<int> Get::getAlbumRandList() {
    // SELECT album_id FROM album ORDER BY RANDOM() LIMIT 15
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT 15")
            .arg(LiteralConstant::Column::ALBUM_ID)
            .arg(LiteralConstant::Table::ALBUM);
    return getIntList(sql.toUtf8());
}

QList<int> Get::getArtistRandList() {
    // SELECT artist_id FROM artist ORDER BY RANDOM() LIMIT 15
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT 15")
            .arg(LiteralConstant::Column::ARTIST_ID)
            .arg(LiteralConstant::Table::ARTIST);
    return getIntList(sql.toUtf8());
}

QList<int> Get::getMusicRandList(int length) {
    length = (length == -1) ? 15 : length;

    const QString sql = QString("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT %3")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(length);
    return getIntList(sql.toUtf8());
}

QList<int> Get::getNewMusicList() {
    // SELECT music_id FROM music ORDER BY play_number DESC LIMIT 15
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 15")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::PLAY_NUMBER);
    return getIntList(sql.toUtf8());
}

QList<int> Get::getReadMoreList() {
    // SELECT music_id FROM music ORDER BY insert_time DESC LIMIT 15
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 15")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::LAST_EDIT_TIME); // 请确认 insert_time 对应常量
    return getIntList(sql.toUtf8());
}

QList<int> Get::getIntList(const QString &sql) {
    QList<int> idList;
    try {
        sqlExecuteCallBack(sql.toUtf8(), idListCallBack, &idList);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        return {};
    }
    return idList;
}

MediaData Get::getMediaFromStmt(sqlite3_stmt *stmt) {
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
    data.album = QString::fromUtf8(sqlite3_column_text(stmt, 6));
    data.artist = QString::fromUtf8(sqlite3_column_text(stmt, 7));
    return data;
}

int Get::checkArtistName(const QString &name) {
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        // SELECT COALESCE((SELECT artist_id FROM artist WHERE name = ? LIMIT 1), -1) AS artist_id
        const auto sql = QString("SELECT COALESCE("
                    "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1), "
                    "-1) AS %1")
                .arg(LiteralConstant::Column::ARTIST_ID)
                .arg(LiteralConstant::Table::ARTIST)
                .arg(LiteralConstant::Column::ARTIST_NAME); // 请确认列名
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return r;
}

int Get::checkAlbumName(const QString &name) {
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        // SELECT COALESCE((SELECT album_id FROM album WHERE name = ? LIMIT 1), -1) AS album_id
        const auto sql = QString("SELECT COALESCE("
                    "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1), "
                    "-1) AS %1")
                .arg(LiteralConstant::Column::ALBUM_ID)
                .arg(LiteralConstant::Table::ALBUM)
                .arg(LiteralConstant::Column::ALBUM_NAME);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return r;
}

int Get::checkPlayListName(const QString &name) {
    sqlite3_stmt *stmt = nullptr;
    int r = -1;
    try {
        // SELECT COALESCE((SELECT list_id FROM playlist WHERE name = ? LIMIT 1), -1) AS list_id
        const auto sql = QString("SELECT COALESCE("
                    "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1), "
                    "-1) AS %1")
                .arg(LiteralConstant::Column::PLAYLIST_ID) // 请确认列名
                .arg(LiteralConstant::Table::PLAYLIST)
                .arg(LiteralConstant::Column::PLAYLIST_NAME);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, name);
        stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }
    stmtFree(stmt);
    return r;
}

QStringList Get::getAlbumNameList(const int size, const int start) {
    QStringList albumNameList;
    sqlite3_stmt *stmt = nullptr;
    try {
        // SELECT name FROM album ORDER by name LIMIT ? OFFSET ?
        const auto sql = QString("SELECT %1 FROM %2 ORDER by %1 LIMIT ? OFFSET ?")
                .arg(LiteralConstant::Column::ALBUM_NAME)
                .arg(LiteralConstant::Table::ALBUM);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, size);
        stmtBindInt(stmt, 2, start);
        while (stmtStep(stmt)) {
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            albumNameList.append(name);
        };
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return albumNameList;
}

QStringList Get::getArtistNameList(const int size, const int start) {
    QStringList artistNameList;
    sqlite3_stmt *stmt = nullptr;
    try {
        // SELECT name FROM artist ORDER by name LIMIT ? OFFSET ?
        const auto sql = QString("SELECT %1 FROM %2 ORDER by %1 LIMIT ? OFFSET ?")
                .arg(LiteralConstant::Column::ARTIST_NAME)
                .arg(LiteralConstant::Table::ARTIST);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindInt(stmt, 1, size);
        stmtBindInt(stmt, 2, start);
        while (stmtStep(stmt)) {
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            artistNameList.append(name);
        };
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return artistNameList;
}


/**
 * @brief 生成用于获取按指定排序规则排序的音乐 ID 列表的 SQL 查询语句
 *
 * 该函数根据传入的排序类型，动态构建不同的 SQL 查询，用于从数据库中筛选出属于
 * 特定主表（如歌单、专辑或艺术家）的音乐记录，并按照指定的列进行排序。生成的 SQL
 * 语句包含占位符，便于后续参数绑定，主要用于分页查询（LIMIT 和 OFFSET）。
 *
 * @param sort           排序类型
 * @param masterTable    主表名
 * @param masterColumn   主表中的主键名
 * @param isLimit
 * @return QString       生成的 SQL 查询语句，具体格式取决于排序类型：
 *
 */
QString Get::getSelectMusicSortSql(const int sort, const QString &masterTable, const QString &masterColumn,
                                   bool isLimit) {
    QString aimTable;
    QString aimColumn;
    QString aimLinkTable;
    QString sortDic;
    QString orderColumn;
    bool isOnlyMusic = true;

    switch (static_cast<SORT_TYPE>(sort)) {
        case SORT_ALBUM_ASC:
            aimTable = LiteralConstant::Table::ALBUM;
            aimColumn = LiteralConstant::Column::ALBUM_ID;
            aimLinkTable = LiteralConstant::Table::ALBUM_MUSIC;
            orderColumn = LiteralConstant::Column::ALBUM_NAME;
            sortDic = LiteralConstant::ASC;
            isOnlyMusic = false;
            break;
        case SORT_ALBUM_DESC:
            aimTable = LiteralConstant::Table::ALBUM;
            aimColumn = LiteralConstant::Column::ALBUM_ID;
            aimLinkTable = LiteralConstant::Table::ALBUM_MUSIC;
            orderColumn = LiteralConstant::Column::ALBUM_NAME;
            sortDic = LiteralConstant::DESC;
            isOnlyMusic = false;
            break;
        case SORT_ARTIST_ASC:
            aimTable = LiteralConstant::Table::ARTIST;
            aimColumn = LiteralConstant::Column::ARTIST_ID;
            aimLinkTable = LiteralConstant::Table::ARTIST_MUSIC;
            orderColumn = LiteralConstant::Column::ARTIST_NAME;
            sortDic = LiteralConstant::ASC;
            isOnlyMusic = false;
            break;
        case SORT_ARTIST_DESC:
            aimTable = LiteralConstant::Table::ARTIST;
            aimColumn = LiteralConstant::Column::ARTIST_ID;
            aimLinkTable = LiteralConstant::Table::ARTIST_MUSIC;
            orderColumn = LiteralConstant::Column::ARTIST_NAME;
            sortDic = LiteralConstant::DESC;
            isOnlyMusic = false;
            break;
        case SORT_DURATION_ASC:
            orderColumn = LiteralConstant::Column::DURATION;
            sortDic = LiteralConstant::ASC;
            break;
        case SORT_DURATION_DESC:
            orderColumn = LiteralConstant::Column::DURATION;
            sortDic = LiteralConstant::DESC;
            break;
        case SORT_LEVEL_ASC:
            orderColumn = LiteralConstant::Column::LEVEL;
            sortDic = LiteralConstant::ASC;
            break;
        case SORT_LEVEL_DESC:
            orderColumn = LiteralConstant::Column::LEVEL;
            sortDic = LiteralConstant::DESC;
            break;
        case SORT_LAST_EDIT_TIME_ASC:
            orderColumn = LiteralConstant::Column::LAST_EDIT_TIME;
            sortDic = LiteralConstant::ASC;
            break;
        case SORT_LAST_EDIT_TIME_DESC:
            orderColumn = LiteralConstant::Column::LAST_EDIT_TIME;
            sortDic = LiteralConstant::DESC;
            break;
        case SORT_PLAY_NUMBER_ASC:
            orderColumn = LiteralConstant::Column::PLAY_NUMBER;
            sortDic = LiteralConstant::ASC;
            break;
        case SORT_PLAY_NUMBER_DESC:
            orderColumn = LiteralConstant::Column::PLAY_NUMBER;
            sortDic = LiteralConstant::DESC;
            break;
        case SORT_TITTLE_ASC:
            orderColumn = LiteralConstant::Column::TITLE;
            sortDic = LiteralConstant::ASC;
            break;
        case SORT_TITTLE_DESC:
        default:
            orderColumn = LiteralConstant::Column::TITLE;
            sortDic = LiteralConstant::DESC;
            break;
    }

    QString limit = "";
    if (isLimit) {
        limit = "LIMIT ? OFFSET ?";
    }

    // 按 music 表的列排序，需要连接 album_music 或 artist_music
    QString sql;
    if (isOnlyMusic) {
        // SELECT aim.music_id
        // FROM music aim
        // JOIN master ON master.music_id = aim.music_id
        // WHERE master.masterColumn = ?
        // ORDER BY aim.orderColumn sortDic
        // LIMIT ? OFFSET ?
        sql = QString("SELECT GROUP_CONCAT(aim.%1) as musicList "
                    "FROM %2 aim "
                    "JOIN %3 master ON master.%1 = aim.%1 "
                    "WHERE master.%4 = ? "
                    "ORDER BY aim.%5 %6 "
                    "%7")
                .arg(LiteralConstant::Column::MUSIC_ID) // %1 music_id
                .arg(LiteralConstant::Table::MUSIC) // %2 music
                .arg(masterTable) // %3 album/artist
                .arg(masterColumn) // %4 album/artist
                .arg(orderColumn) // %5 排序列名
                .arg(sortDic) // %6 ASC/DESC
                .arg(limit); //7
    } else {
        // SELECT aim_link.music_id
        // FROM aimLinkTable aim_link
        // JOIN aimTable aim ON aim.aimColumn = aim_link.aimColumn
        // JOIN masterTable master ON master.music_id = aim_link.music_id
        // WHERE master.masterColumn = ?
        // ORDER BY aim_link.orderColumn sortDic
        // LIMIT ? OFFSET ?
        sql = QString("SELECT GROUP_CONCAT(aim_link.%1) as musicList "
                    "FROM %2 aim_link "
                    "JOIN %3 aim ON aim.%4 = aim_link.%4 "
                    "JOIN %5 master ON master.%1 = aim_link.%1 "
                    "WHERE master.%6 = ? "
                    "ORDER BY aim.%7 %8 "
                    "%9")
                .arg(LiteralConstant::Column::MUSIC_ID) // %1 music_id
                .arg(aimLinkTable) // %2 album_music/artist_music
                .arg(aimTable) // %3
                .arg(aimColumn) // %4 通常是 album 或 artist
                .arg(masterTable) // %5
                .arg(masterColumn) // %6
                .arg(orderColumn) // %7
                .arg(sortDic) // %8
                .arg(limit); //9
    }

    return sql;
}