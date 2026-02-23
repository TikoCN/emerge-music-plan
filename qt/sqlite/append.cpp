#include "append.h"
#include <QDir>
#include "namekey.h"
#include "baseclass/dataexception.h"

bool Append::appendMusic(const MediaData &data) {
    QList<MediaData> dataList;
    dataList.append(data);
    return appendMusic(dataList);
}

bool Append::appendMusic(const QList<MediaData> &dataList) {
    bool result = true;
    sqlite3_stmt *appendMusicStmt = nullptr;
    sqlite3_stmt *getAlbumIDStmt = nullptr;

    try {
        // 构建 SQL：SELECT album_id FROM album WHERE name=? LIMIT 1
        const auto getAlbumIDSql = QString("SELECT %1 FROM %2 WHERE %3=? LIMIT 1")
            .arg(LiteralConstant::Column::ALBUM_ID)
            .arg(LiteralConstant::Table::ALBUM)
            .arg(LiteralConstant::Column::ALBUM_NAME);  // 注意：实际列名可能为 "name"，请确认常量值

        // 构建 SQL：INSERT OR IGNORE INTO music(...) VALUES(...)
        const auto appendMusicSql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5, %6, %7, %8, %9) VALUES(?, ?, ?, ?, ?, ?, ?, ?)")
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::TITLE)
            .arg(LiteralConstant::Column::DURATION)
            .arg(LiteralConstant::Column::LAST_EDIT_TIME)   // insert_time 字段
            .arg(LiteralConstant::Column::LEVEL)
            .arg(LiteralConstant::Column::IS_LOVE)
            .arg(LiteralConstant::Column::PLAY_NUMBER)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::NAME_KEY);        // key 字段，请确认常量值是否为 "key"

        stmtPrepare(&appendMusicStmt, appendMusicSql.toUtf8());
        stmtPrepare(&getAlbumIDStmt, getAlbumIDSql.toUtf8());

        NameKey key(tlog);

        for (const MediaData &data: dataList) {
            stmtReset(getAlbumIDStmt);
            stmtBindText(getAlbumIDStmt, 1, data.album);
            stmtStep(getAlbumIDStmt);   // 注意：此查询结果未使用，可能是遗留代码

            stmtReset(appendMusicStmt);
            stmtBindText(appendMusicStmt, 1, data.title);
            stmtBindLong(appendMusicStmt, 2, data.duration);
            stmtBindLong(appendMusicStmt, 3, QDateTime::currentMSecsSinceEpoch());
            stmtBindInt(appendMusicStmt, 4, data.level);
            stmtBindInt(appendMusicStmt, 5, data.isLove);
            stmtBindInt(appendMusicStmt, 6, data.playNumber);
            stmtBindText(appendMusicStmt, 7, data.url);
            stmtBindText(appendMusicStmt, 8, key.find(data.title));
            stmtStep(appendMusicStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendMusicStmt);
    stmtFree(getAlbumIDStmt);
    return result;
}

bool Append::appendAlbum(const QString &album) {
    QList<QString> dataList;
    dataList.append(album);
    return appendAlbum(dataList);
}

bool Append::appendAlbum(const QStringList &albumList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 构建 SQL：INSERT OR IGNORE INTO album(name, key, sort) VALUES(?, ?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4) VALUES(?, ?, ?)")
            .arg(LiteralConstant::Table::ALBUM)
            .arg(LiteralConstant::Column::ALBUM_NAME)   // 注意：实际列名可能为 "name"
            .arg(LiteralConstant::Column::NAME_KEY)     // key 字段
            .arg(LiteralConstant::Column::SORT);
        stmtPrepare(&stmt, sql.toUtf8());
        NameKey key(tlog);

        for (const QString &i: albumList) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, i);
            stmtBindText(stmt, 2, key.find(i));
            stmtBindInt(stmt, 3, 1);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendAlbumMusic(int id, const QList<int> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 构建 SQL：INSERT OR IGNORE INTO album_music(album_id, music_id) VALUES(?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::ALBUM_MUSIC)
            .arg(LiteralConstant::Column::ALBUM_ID)
            .arg(LiteralConstant::Column::MUSIC_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, i);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendAlbumMusic(const QPair<QString, QString> &pair) {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendAlbumMusic(dataList);
}

bool Append::appendAlbumMusic(const QList<QPair<QString, QString>> &pairList) {
    bool result = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;

    try {
        // 构建 getIdSql：获取 music_id 和 album_id
        const auto getIdSql = QString("SELECT"
                "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                "(SELECT %4 FROM %5 WHERE %6 = ? LIMIT 1) AS %4")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::ALBUM_ID)
            .arg(LiteralConstant::Table::ALBUM)
            .arg(LiteralConstant::Column::ALBUM_NAME);   // 注意：实际列名可能为 "name"

        // 构建 appendSql：INSERT OR IGNORE INTO album_music(album_id, music_id) VALUES(?, ?)
        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::ALBUM_MUSIC)
            .arg(LiteralConstant::Column::ALBUM_ID)
            .arg(LiteralConstant::Column::MUSIC_ID);

        stmtPrepare(&appendStmt, appendSql.toUtf8());
        stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, fst);
            stmtBindText(getIdStmt, 2, snd);
            stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int album_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, album_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return result;
}

bool Append::appendArtist(const QString &artist) {
    QList<QString> dataList;
    dataList.append(artist);
    return appendArtist(dataList);
}

bool Append::appendArtist(const QStringList &artistList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 构建 SQL：INSERT OR IGNORE INTO artist(name, key, sort) VALUES(?, ?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4) VALUES(?, ?, ?)")
            .arg(LiteralConstant::Table::ARTIST)
            .arg(LiteralConstant::Column::ARTIST_NAME)   // 注意：实际列名可能为 "name"
            .arg(LiteralConstant::Column::NAME_KEY)      // key 字段
            .arg(LiteralConstant::Column::SORT);
        stmtPrepare(&stmt, sql.toUtf8());
        NameKey key(tlog);

        for (const QString &i: artistList) {
            qDebug() << i;
            stmtReset(stmt);
            stmtBindText(stmt, 1, i);
            stmtBindText(stmt, 2, key.find(i));
            stmtBindInt(stmt, 3, 1);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendArtistMusic(const int id, const QList<int> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 构建 SQL：INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::ARTIST_MUSIC)
            .arg(LiteralConstant::Column::ARTIST_ID)
            .arg(LiteralConstant::Column::MUSIC_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, i);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendArtistMusic(const QPair<QString, QString> &pair) {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendArtistMusic(dataList);
}

bool Append::appendArtistMusic(const QList<QPair<QString, QString> > &pairList) {
    bool result = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;

    try {
        // 构建 getIdSql：获取 music_id 和 artist_id
        const auto getIdSql = QString("SELECT"
                "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                "(SELECT %4 FROM %5 WHERE %6 = ? LIMIT 1) AS %4")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::ARTIST_ID)
            .arg(LiteralConstant::Table::ARTIST)
            .arg(LiteralConstant::Column::ARTIST_NAME);   // 注意：实际列名可能为 "name"

        // 构建 appendSql：INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)
        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::ARTIST_MUSIC)
            .arg(LiteralConstant::Column::ARTIST_ID)
            .arg(LiteralConstant::Column::MUSIC_ID);

        stmtPrepare(&appendStmt, appendSql.toUtf8());
        stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, fst);
            stmtBindText(getIdStmt, 2, snd);
            stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int artist_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, artist_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return result;
}

bool Append::appendDirPlayList(const QString &url) {
    QList<QString> dataList;
    dataList.append(url);
    return appendDirPlayList(dataList);
}

bool Append::appendDirPlayList(const QStringList &urlList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 构建 SQL：INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
            .arg(LiteralConstant::Table::PLAYLIST)
            .arg(LiteralConstant::Column::PLAYLIST_NAME)   // 注意：实际列名可能为 "name"
            .arg(LiteralConstant::Column::SORT)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::IS_DIR);
        stmtPrepare(&stmt, sql.toUtf8());
        for (const QString &i: urlList) {
            stmtReset(stmt);
            QDir dir(i);

            stmtBindText(stmt, 1, dir.dirName());
            stmtBindInt(stmt, 2, 0);
            stmtBindText(stmt, 3, i);
            stmtBindInt(stmt, 4, 1);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendUserPlayList(const QString &name) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 构建 SQL：INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
            .arg(LiteralConstant::Table::PLAYLIST)
            .arg(LiteralConstant::Column::PLAYLIST_NAME)   // 注意：实际列名可能为 "name"
            .arg(LiteralConstant::Column::SORT)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::IS_DIR);
        stmtPrepare(&stmt, sql.toUtf8());
        stmtBindText(stmt, 1, name);
        stmtBindInt(stmt, 2, 0);
        stmtBindText(stmt, 3, "none");
        stmtBindInt(stmt, 4, 1);
        stmtStep(stmt);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const int id, const QList<int> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 注意：原 SQL 使用了 "list_music"，根据常量应为 playlist_music
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
            .arg(LiteralConstant::Column::PLAYLIST_ID)   // 注意：实际列名可能为 "list_id"
            .arg(LiteralConstant::Column::MUSIC_ID);
        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, i);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const QPair<QString, QString> &pair) {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendPlayListMusic(dataList);
}

bool Append::appendPlayListMusic(const QList<QPair<QString, QString> > &pairList) {
    bool result = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;

    try {
        // 构建 getIdSql：获取 music_id 和 list_id
        const auto getIdSql = QString("SELECT"
                "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                "(SELECT %4 FROM %5 WHERE %3 = ? LIMIT 1) AS %4")
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::PLAYLIST_ID)   // 注意：实际列名可能为 "list_id"
            .arg(LiteralConstant::Table::PLAYLIST);

        // 构建 appendSql：INSERT OR IGNORE INTO playlist_music(list_id, music_id) VALUES(?, ?)
        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
            .arg(LiteralConstant::Column::PLAYLIST_ID)   // 注意：实际列名可能为 "list_id"
            .arg(LiteralConstant::Column::MUSIC_ID);

        stmtPrepare(&appendStmt, appendSql.toUtf8());
        stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, fst);
            stmtBindText(getIdStmt, 2, snd);
            stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int list_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, list_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return result;
}

bool Append::appendPlayingListMusic(int musicId, int position) {
    QList<int> list;
    list.append(musicId);
    return appendPlayingListMusic(list, position);
}

bool Append::appendPlayingListMusic(const QList<int> &musicList, int start) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        // 构建 SQL：INSERT OR IGNORE INTO playinglist(music_id, position) VALUES(?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
            .arg(LiteralConstant::Table::PLAYINGLIST)
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Column::POSITION);
        stmtPrepare(&stmt, sql.toUtf8());

        for (int i = 0; i < musicList.length(); i++) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, musicList[i]);
            stmtBindInt(stmt, 2, start + i);
            stmtStep(stmt);
        }

    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    return result;
}
