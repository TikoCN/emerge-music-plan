#include "Append.h"
#include <QDir>
#include "namekey.h"
#include "baseclass/DataException.h"

bool Append::appendMusic(const MediaData &data) const {
    QList<MediaData> dataList;
    dataList.append(data);
    return appendMusic(dataList);
}

bool Append::appendMusic(const QList<MediaData> &dataList) const {
    bool          result          = true;
    sqlite3_stmt *appendMusicStmt = nullptr;
    sqlite3_stmt *getAlbumIDStmt  = nullptr;

    try {
        core->begin();

        // 构建 SQL：SELECT album_id FROM album WHERE name=? LIMIT 1
        const auto getAlbumIDSql = QString("SELECT %1 FROM %2 WHERE %3=? LIMIT 1")
                                  .arg(LiteralConstant::Column::ALBUM_ID)
                                  .arg(LiteralConstant::Table::ALBUM)
                                  .arg(LiteralConstant::Column::ALBUM_NAME); // 注意：实际列名可能为 "name"，请确认常量值

        // 构建 SQL：INSERT OR IGNORE INTO music(...) VALUES(...)
        const auto appendMusicSql = QString(
                                        "INSERT OR IGNORE INTO %1(%2, %3, %4, %5, %6, %7, %8, %9) VALUES(?, ?, ?, ?, ?, ?, ?, ?)")
                                   .arg(LiteralConstant::Table::MUSIC)
                                   .arg(LiteralConstant::Column::TITLE)
                                   .arg(LiteralConstant::Column::DURATION)
                                   .arg(LiteralConstant::Column::LAST_EDIT_TIME) // insert_time 字段
                                   .arg(LiteralConstant::Column::LEVEL)
                                   .arg(LiteralConstant::Column::IS_LOVE)
                                   .arg(LiteralConstant::Column::PLAY_NUMBER)
                                   .arg(LiteralConstant::Column::URL)
                                   .arg(LiteralConstant::Column::NAME_KEY); // key 字段，请确认常量值是否为 "key"

        core->stmtPrepare(&appendMusicStmt, appendMusicSql.toUtf8());
        core->stmtPrepare(&getAlbumIDStmt, getAlbumIDSql.toUtf8());

        NameKey key;

        for (const MediaData &data: dataList) {
            core->stmtReset(getAlbumIDStmt);
            core->stmtBindText(getAlbumIDStmt, 1, data.album);
            core->stmtStep(getAlbumIDStmt); // 注意：此查询结果未使用，可能是遗留代码

            core->stmtReset(appendMusicStmt);
            core->stmtBindText(appendMusicStmt, 1, data.title);
            core->stmtBindLong(appendMusicStmt, 2, data.duration);
            core->stmtBindLong(appendMusicStmt, 3, QDateTime::currentMSecsSinceEpoch());
            core->stmtBindInt(appendMusicStmt, 4, data.level);
            core->stmtBindInt(appendMusicStmt, 5, data.isLove);
            core->stmtBindInt(appendMusicStmt, 6, data.playNumber);
            core->stmtBindText(appendMusicStmt, 7, data.url);
            core->stmtBindText(appendMusicStmt, 8, key.find(data.title));
            core->stmtStep(appendMusicStmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(appendMusicStmt);
    Core::stmtFree(getAlbumIDStmt);
    return result;
}

bool Append::appendAlbum(const QString &album) const {
    QList<QString> dataList;
    dataList.append(album);
    return appendAlbum(dataList);
}

bool Append::appendAlbum(const QStringList &albumList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        core->begin();

        // 构建 SQL：INSERT OR IGNORE INTO album(name, key, sort) VALUES(?, ?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4) VALUES(?, ?, ?)")
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::ALBUM_NAME) // 注意：实际列名可能为 "name"
                        .arg(LiteralConstant::Column::NAME_KEY)   // key 字段
                        .arg(LiteralConstant::Column::SORT);
        core->stmtPrepare(&stmt, sql.toUtf8());
        NameKey key;

        for (const QString &i: albumList) {
            core->stmtReset(stmt);
            core->stmtBindText(stmt, 1, i);
            core->stmtBindText(stmt, 2, key.find(i));
            core->stmtBindInt(stmt, 3, 1);
            core->stmtStep(stmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendAlbumMusic(int id, const QList<int> &musicList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();

        // 构建 SQL：INSERT OR IGNORE INTO album_music(album_id, music_id) VALUES(?, ?)
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                        .arg(LiteralConstant::Table::ALBUM_MUSIC)
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, i);
            core->stmtStep(stmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendAlbumMusic(const QPair<QString, QString> &pair) const {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendAlbumMusic(dataList);
}

bool Append::appendAlbumMusic(const QList<QPair<QString, QString> > &pairList) const {
    bool          result     = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt  = nullptr;

    try {
        core->begin();

        // 构建 getIdSql：获取 music_id 和 album_id
        const auto getIdSql = QString("SELECT"
                                  "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                                  "(SELECT %4 FROM %5 WHERE %6 = ? LIMIT 1) AS %4")
                             .arg(LiteralConstant::Column::MUSIC_ID)
                             .arg(LiteralConstant::Table::MUSIC)
                             .arg(LiteralConstant::Column::URL)
                             .arg(LiteralConstant::Column::ALBUM_ID)
                             .arg(LiteralConstant::Table::ALBUM)
                             .arg(LiteralConstant::Column::ALBUM_NAME);

        // 构建 appendSql：INSERT OR IGNORE INTO album_music(album_id, music_id) VALUES(?, ?)
        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::ALBUM_MUSIC)
                              .arg(LiteralConstant::Column::ALBUM_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&appendStmt, appendSql.toUtf8());
        core->stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            core->stmtReset(getIdStmt);
            core->stmtBindText(getIdStmt, 1, fst);
            core->stmtBindText(getIdStmt, 2, snd);
            core->stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int album_id = sqlite3_column_int(getIdStmt, 1);
            core->stmtReset(appendStmt);
            core->stmtBindInt(appendStmt, 1, album_id);
            core->stmtBindInt(appendStmt, 2, music_id);
            core->stmtStep(appendStmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(appendStmt);
    Core::stmtFree(getIdStmt);
    return result;
}

bool Append::appendArtist(const QString &artist) const {
    QList<QString> dataList;
    dataList.append(artist);
    return appendArtist(dataList);
}

bool Append::appendArtist(const QStringList &artistList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        core->begin();
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4) VALUES(?, ?, ?)")
                        .arg(LiteralConstant::Table::ARTIST)
                        .arg(LiteralConstant::Column::ARTIST_NAME)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::SORT);
        core->stmtPrepare(&stmt, sql.toUtf8());
        NameKey key;

        for (const QString &i: artistList) {
            core->stmtReset(stmt);
            core->stmtBindText(stmt, 1, i);
            core->stmtBindText(stmt, 2, key.find(i));
            core->stmtBindInt(stmt, 3, 1);
            core->stmtStep(stmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendArtistMusic(const int id, const QList<int> &musicList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                        .arg(LiteralConstant::Table::ARTIST_MUSIC)
                        .arg(LiteralConstant::Column::ARTIST_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, i);
            core->stmtStep(stmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendArtistMusic(const QPair<QString, QString> &pair) const {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendArtistMusic(dataList);
}

bool Append::appendArtistMusic(const QList<QPair<QString, QString> > &pairList) const {
    bool          result     = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt  = nullptr;

    try {
        core->begin();
        const auto getIdSql = QString("SELECT"
                                  "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                                  "(SELECT %4 FROM %5 WHERE %6 = ? LIMIT 1) AS %4")
                             .arg(LiteralConstant::Column::MUSIC_ID)
                             .arg(LiteralConstant::Table::MUSIC)
                             .arg(LiteralConstant::Column::URL)
                             .arg(LiteralConstant::Column::ARTIST_ID)
                             .arg(LiteralConstant::Table::ARTIST)
                             .arg(LiteralConstant::Column::ARTIST_NAME);

        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::ARTIST_MUSIC)
                              .arg(LiteralConstant::Column::ARTIST_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&appendStmt, appendSql.toUtf8());
        core->stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            core->stmtReset(getIdStmt);
            core->stmtBindText(getIdStmt, 1, fst);
            core->stmtBindText(getIdStmt, 2, snd);
            core->stmtStep(getIdStmt);

            const int music_id  = sqlite3_column_int(getIdStmt, 0);
            const int artist_id = sqlite3_column_int(getIdStmt, 1);
            core->stmtReset(appendStmt);
            core->stmtBindInt(appendStmt, 1, artist_id);
            core->stmtBindInt(appendStmt, 2, music_id);
            core->stmtStep(appendStmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(appendStmt);
    Core::stmtFree(getIdStmt);
    return result;
}

bool Append::appendDirPlayList(const QString &url) const {
    QList<QString> dataList;
    dataList.append(url);
    return appendDirPlayList(dataList);
}

bool Append::appendDirPlayList(const QStringList &urlList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        core->begin();
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::SORT)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::IS_DIR);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const QString &i: urlList) {
            core->stmtReset(stmt);
            QDir dir(i);

            core->stmtBindText(stmt, 1, dir.dirName());
            core->stmtBindInt(stmt, 2, 0);
            core->stmtBindText(stmt, 3, i);
            core->stmtBindInt(stmt, 4, 1);
            core->stmtStep(stmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendUserPlayList(const QString &name) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::SORT)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::IS_DIR);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindText(stmt, 1, name);
        core->stmtBindInt(stmt, 2, 0);
        core->stmtBindText(stmt, 3, "none");
        core->stmtBindInt(stmt, 4, 0);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const int id, const QList<int> &musicList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                        .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, i);
            core->stmtStep(stmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const QPair<QString, QString> &pair) const {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendPlayListMusic(dataList);
}

bool Append::appendPlayListMusic(const QList<QPair<QString, QString> > &pairList) const {
    bool          result     = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt  = nullptr;

    try {
        core->begin();
        const auto getIdSql = QString("SELECT"
                                  "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1) AS %1,"
                                  "(SELECT %4 FROM %5 WHERE %3 = ? LIMIT 1) AS %4")
                             .arg(LiteralConstant::Column::MUSIC_ID)
                             .arg(LiteralConstant::Table::MUSIC)
                             .arg(LiteralConstant::Column::URL)
                             .arg(LiteralConstant::Column::PLAYLIST_ID)
                             .arg(LiteralConstant::Table::PLAYLIST);

        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                              .arg(LiteralConstant::Column::PLAYLIST_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&appendStmt, appendSql.toUtf8());
        core->stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &[fst, snd]: pairList) {
            core->stmtReset(getIdStmt);
            core->stmtBindText(getIdStmt, 1, fst);
            core->stmtBindText(getIdStmt, 2, snd);
            core->stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int list_id  = sqlite3_column_int(getIdStmt, 1);
            core->stmtReset(appendStmt);
            core->stmtBindInt(appendStmt, 1, list_id);
            core->stmtBindInt(appendStmt, 2, music_id);
            core->stmtStep(appendStmt);
        }
        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(appendStmt);
    Core::stmtFree(getIdStmt);
    return result;
}

bool Append::appendPlayingListMusic(int musicId, int position) const {
    QList<int> list;
    list.append(musicId);
    return appendPlayingListMusic(list, position);
}

bool Append::appendPlayingListMusic(const QList<int> &musicList, int start) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        core->begin();

        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, sql.toUtf8());

        for (int i = 0; i < musicList.length(); i++) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, musicList[i]);
            core->stmtBindInt(stmt, 2, start + i);
            core->stmtStep(stmt);
        }

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const {
    bool          result     = true;
    sqlite3_stmt *stmt       = nullptr;
    int           artistId   = -1;
    int           playListId = -1;

    try {
        core->begin();

        const auto getArtistIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                   .arg(LiteralConstant::Column::ARTIST_ID)
                                   .arg(LiteralConstant::Table::ARTIST)
                                   .arg(LiteralConstant::Column::ARTIST_NAME);

        const auto getPlayListIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                     .arg(LiteralConstant::Column::PLAYLIST_ID)
                                     .arg(LiteralConstant::Table::PLAYLIST)
                                     .arg(LiteralConstant::Column::PLAYLIST_NAME);

        core->stmtPrepare(&stmt, getArtistIdSql.toUtf8());
        core->stmtBindText(stmt, 1, artistName);
        if (core->stmtStep(stmt)) {
            artistId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        core->stmtPrepare(&stmt, getPlayListIdSql.toUtf8());
        core->stmtBindText(stmt, 1, playListName);
        if (core->stmtStep(stmt)) {
            playListId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (artistId == -1) {
            throw DataException("歌手不存在");
        }
        if (playListId == -1) {
            throw DataException("播放列表不存在");
        }

        const auto appendSql = QString(
                                   "INSERT OR IGNORE INTO %1(%2, %3) "
                                   "SELECT ?, %3 FROM %4 WHERE %5 = ?")
                              .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                              .arg(LiteralConstant::Column::PLAYLIST_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID)
                              .arg(LiteralConstant::Table::ARTIST_MUSIC)
                              .arg(LiteralConstant::Column::ARTIST_ID);
        core->stmtPrepare(&stmt, appendSql.toUtf8());
        core->stmtBindInt(stmt, 1, playListId);
        core->stmtBindInt(stmt, 2, artistId);
        core->stmtStep(stmt);

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const {
    bool          result     = true;
    sqlite3_stmt *stmt       = nullptr;
    int           albumId    = -1;
    int           playListId = -1;

    try {
        core->begin();

        const auto getAlbumIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                  .arg(LiteralConstant::Column::ALBUM_ID)
                                  .arg(LiteralConstant::Table::ALBUM)
                                  .arg(LiteralConstant::Column::ALBUM_NAME);

        const auto getPlayListIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                     .arg(LiteralConstant::Column::PLAYLIST_ID)
                                     .arg(LiteralConstant::Table::PLAYLIST)
                                     .arg(LiteralConstant::Column::PLAYLIST_NAME);

        core->stmtPrepare(&stmt, getAlbumIdSql.toUtf8());
        core->stmtBindText(stmt, 1, albumName);
        if (core->stmtStep(stmt)) {
            albumId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        core->stmtPrepare(&stmt, getPlayListIdSql.toUtf8());
        core->stmtBindText(stmt, 1, playListName);
        if (core->stmtStep(stmt)) {
            playListId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (albumId == -1) {
            throw DataException("专辑不存在");
        }
        if (playListId == -1) {
            throw DataException("播放列表不存在");
        }

        const auto appendSql = QString(
                                   "INSERT OR IGNORE INTO %1(%2, %3) "
                                   "SELECT ?, %3 FROM %4 WHERE %5 = ?")
                              .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                              .arg(LiteralConstant::Column::PLAYLIST_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID)
                              .arg(LiteralConstant::Table::ALBUM_MUSIC)
                              .arg(LiteralConstant::Column::ALBUM_ID);
        core->stmtPrepare(&stmt, appendSql.toUtf8());
        core->stmtBindInt(stmt, 1, playListId);
        core->stmtBindInt(stmt, 2, albumId);
        core->stmtStep(stmt);

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}

bool Append::addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const {
    bool          result           = true;
    sqlite3_stmt *stmt             = nullptr;
    int           sourcePlayListId = -1;
    int           targetPlayListId = -1;

    try {
        core->begin();

        const auto getPlayListIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                     .arg(LiteralConstant::Column::PLAYLIST_ID)
                                     .arg(LiteralConstant::Table::PLAYLIST)
                                     .arg(LiteralConstant::Column::PLAYLIST_NAME);

        core->stmtPrepare(&stmt, getPlayListIdSql.toUtf8());
        core->stmtBindText(stmt, 1, sourcePlayListName);
        if (core->stmtStep(stmt)) {
            sourcePlayListId = sqlite3_column_int(stmt, 0);
        }
        core->stmtReset(stmt);

        core->stmtBindText(stmt, 1, targetPlayListName);
        if (core->stmtStep(stmt)) {
            targetPlayListId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (sourcePlayListId == -1) {
            throw DataException("源播放列表不存在");
        }
        if (targetPlayListId == -1) {
            throw DataException("目标播放列表不存在");
        }

        const auto appendSql = QString(
                                   "INSERT OR IGNORE INTO %1(%2, %3) "
                                   "SELECT ?, %3 FROM %4 WHERE %2 = ?")
                              .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                              .arg(LiteralConstant::Column::PLAYLIST_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID)
                              .arg(LiteralConstant::Table::PLAYLIST_MUSIC);
        core->stmtPrepare(&stmt, appendSql.toUtf8());
        core->stmtBindInt(stmt, 1, targetPlayListId);
        core->stmtBindInt(stmt, 2, sourcePlayListId);
        core->stmtStep(stmt);

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    Core::stmtFree(stmt);
    return result;
}
