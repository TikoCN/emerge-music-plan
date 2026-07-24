#include "AlbumRepository.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "baseclass/DataException.h"
#include "baseclass/SortType.h"
#include "NameKey.h"

QStringList AlbumRepository::getKeys() const {
    QStringList keyList;
    try {
        const auto sql = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1 ASC")
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Table::ALBUM);

        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)-> int {
            auto *strings = static_cast<QStringList *>(data);
            strings->append(QString(*argv));
            return SQLITE_OK;
        };

        core->sqlExecuteCallBack(sql.toUtf8(), callback, &keyList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return keyList;
    }
    return keyList;
}

QList<int> AlbumRepository::getByKey(const QString &key, int size, int start) const {
    QList<int>    albumList;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::NAME_KEY);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindText(stmt, 1, key);
        core->stmtBindInt(stmt, 2, size);
        core->stmtBindInt(stmt, 3, start);
        while (core->stmtStep(stmt)) {
            const int id = sqlite3_column_int(stmt, 0);
            albumList.append(id);
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        albumList.clear();
    }
    Core::stmtFree(stmt);
    return albumList;
}

AlbumPtr AlbumRepository::get(const int id) const {
    sqlite3_stmt *stmt  = nullptr;
    AlbumPtr      album = nullptr;

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
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::ALBUM_NAME)
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::SORT)
                        .arg(LiteralConstant::Table::ALBUM_MUSIC)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::MUSIC)
                        .arg(LiteralConstant::Column::DURATION)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::ALBUM_MUSIC)
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());

        core->stmtReset(stmt);
        core->stmtBindInt(stmt, 1, id);
        core->stmtStep(stmt);
        album             = AlbumPtr(new Album());
        album->name       = QString::fromUtf8(sqlite3_column_text(stmt, 0));
        album->id         = sqlite3_column_int(stmt, 1);
        album->nameKey    = QString::fromUtf8(sqlite3_column_text(stmt, 2));
        album->sort       = static_cast<SORT_TYPE>(sqlite3_column_int(stmt, 3));
        album->musicCount = sqlite3_column_int(stmt, 4);
        album->duration   = sqlite3_column_int64(stmt, 5);
        album->firstMusic = sqlite3_column_int(stmt, 6);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        album = nullptr;
    }
    Core::stmtFree(stmt);
    return album;
}

QList<int> AlbumRepository::getMusic(const int id, const int size, const int start, const int sort) const {
    QList<int>    list;
    sqlite3_stmt *stmt = nullptr;
    try {
        QString orderColumn;
        QString sortDic;
        bool    isOnlyMusic = true;

        switch (static_cast<SORT_TYPE>(sort)) {
            case SORT_ALBUM_ASC:
            case SORT_ALBUM_DESC:
            case SORT_ARTIST_ASC:
            case SORT_ARTIST_DESC:
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

        QString sql;
        if (isOnlyMusic) {
            sql = QString("SELECT aim.%1 "
                      "FROM %2 aim "
                      "JOIN %3 master ON master.%1 = aim.%1 "
                      "WHERE master.%4 = ? "
                      "ORDER BY aim.%5 %6 "
                      "LIMIT ? OFFSET ?")
                 .arg(LiteralConstant::Column::MUSIC_ID)
                 .arg(LiteralConstant::Table::MUSIC)
                 .arg(LiteralConstant::Table::ALBUM_MUSIC)
                 .arg(LiteralConstant::Column::ALBUM_ID)
                 .arg(orderColumn)
                 .arg(sortDic);
        } else {
            QString aimTable, aimColumn, aimLinkTable;
            if (sort == SORT_ALBUM_ASC || sort == SORT_ALBUM_DESC) {
                aimTable     = LiteralConstant::Table::ALBUM;
                aimColumn    = LiteralConstant::Column::ALBUM_ID;
                aimLinkTable = LiteralConstant::Table::ALBUM_MUSIC;
                orderColumn  = LiteralConstant::Column::ALBUM_NAME;
            } else {
                aimTable     = LiteralConstant::Table::ARTIST;
                aimColumn    = LiteralConstant::Column::ARTIST_ID;
                aimLinkTable = LiteralConstant::Table::ARTIST_MUSIC;
                orderColumn  = LiteralConstant::Column::ARTIST_NAME;
            }
            sql = QString("SELECT aim_link.%1 "
                      "FROM %2 aim_link "
                      "JOIN %3 aim ON aim.%4 = aim_link.%4 "
                      "JOIN %5 master ON master.%1 = aim_link.%1 "
                      "WHERE master.%6 = ? "
                      "ORDER BY aim.%7 %8 "
                      "LIMIT ? OFFSET ?")
                 .arg(LiteralConstant::Column::MUSIC_ID)
                 .arg(aimLinkTable)
                 .arg(aimTable)
                 .arg(aimColumn)
                 .arg(LiteralConstant::Table::ALBUM_MUSIC)
                 .arg(LiteralConstant::Column::ALBUM_ID)
                 .arg(orderColumn)
                 .arg(sortDic);
        }

        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, id);
        core->stmtBindInt(stmt, 2, size);
        core->stmtBindInt(stmt, 3, start);

        while (core->stmtStep(stmt)) {
            list.append(sqlite3_column_int(stmt, 0));
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    return list;
}

QList<int> AlbumRepository::getMusicAll(const int id, const int sort) const {
    QList<int>    list;
    sqlite3_stmt *stmt = nullptr;
    try {
        QString orderColumn;
        QString sortDic;
        bool    isOnlyMusic = true;

        switch (static_cast<SORT_TYPE>(sort)) {
            case SORT_ALBUM_ASC:
            case SORT_ALBUM_DESC:
            case SORT_ARTIST_ASC:
            case SORT_ARTIST_DESC:
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

        QString sql;
        if (isOnlyMusic) {
            sql = QString("SELECT GROUP_CONCAT(aim.%1) as musicList "
                      "FROM %2 aim "
                      "JOIN %3 master ON master.%1 = aim.%1 "
                      "WHERE master.%4 = ? "
                      "ORDER BY aim.%5 %6")
                 .arg(LiteralConstant::Column::MUSIC_ID)
                 .arg(LiteralConstant::Table::MUSIC)
                 .arg(LiteralConstant::Table::ALBUM_MUSIC)
                 .arg(LiteralConstant::Column::ALBUM_ID)
                 .arg(orderColumn)
                 .arg(sortDic);
        } else {
            QString aimTable, aimColumn, aimLinkTable;
            if (sort == SORT_ALBUM_ASC || sort == SORT_ALBUM_DESC) {
                aimTable     = LiteralConstant::Table::ALBUM;
                aimColumn    = LiteralConstant::Column::ALBUM_ID;
                aimLinkTable = LiteralConstant::Table::ALBUM_MUSIC;
                orderColumn  = LiteralConstant::Column::ALBUM_NAME;
            } else {
                aimTable     = LiteralConstant::Table::ARTIST;
                aimColumn    = LiteralConstant::Column::ARTIST_ID;
                aimLinkTable = LiteralConstant::Table::ARTIST_MUSIC;
                orderColumn  = LiteralConstant::Column::ARTIST_NAME;
            }
            sql = QString("SELECT GROUP_CONCAT(aim_link.%1) as musicList "
                      "FROM %2 aim_link "
                      "JOIN %3 aim ON aim.%4 = aim_link.%4 "
                      "JOIN %5 master ON master.%1 = aim_link.%1 "
                      "WHERE master.%6 = ? "
                      "ORDER BY aim.%7 %8")
                 .arg(LiteralConstant::Column::MUSIC_ID)
                 .arg(aimLinkTable)
                 .arg(aimTable)
                 .arg(aimColumn)
                 .arg(LiteralConstant::Table::ALBUM_MUSIC)
                 .arg(LiteralConstant::Column::ALBUM_ID)
                 .arg(orderColumn)
                 .arg(sortDic);
        }

        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, id);
        core->stmtStep(stmt);

        const QStringList strList = QString::fromUtf8(sqlite3_column_text(stmt, 0)).split(",");
        list                      = TypeConversion::sqlStringListToIntList(strList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    return list;
}

int AlbumRepository::getMusicFirst(const int albumId) const {
    int           musicId = -1;
    sqlite3_stmt *stmt    = nullptr;
    try {
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::ALBUM_MUSIC)
                        .arg(LiteralConstant::Column::ALBUM_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, albumId);
        core->stmtStep(stmt);
        musicId = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    return musicId;
}

QList<int> AlbumRepository::getRandList() const {
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT 15")
                    .arg(LiteralConstant::Column::ALBUM_ID)
                    .arg(LiteralConstant::Table::ALBUM);
    QList<int> idList;
    try {
        core->sqlExecuteCallBack(sql.toUtf8(), Core::idListCallBack, &idList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return {};
    }
    return idList;
}

QStringList AlbumRepository::getNameList(const int size, const int start) const {
    QStringList   albumNameList;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = QString("SELECT %1 FROM %2 ORDER by %1 LIMIT ? OFFSET ?")
                        .arg(LiteralConstant::Column::ALBUM_NAME)
                        .arg(LiteralConstant::Table::ALBUM);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, size);
        core->stmtBindInt(stmt, 2, start);
        while (core->stmtStep(stmt)) {
            const QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            albumNameList.append(name);
        };
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    return albumNameList;
}

bool AlbumRepository::append(const QStringList &albumList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;

    try {
        core->begin();

        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4) VALUES(?, ?, ?)")
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::ALBUM_NAME)
                        .arg(LiteralConstant::Column::NAME_KEY)
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

bool AlbumRepository::appendMusic(const QList<MediaData> &dataList) const {
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
                             .arg(LiteralConstant::Column::ALBUM_ID)
                             .arg(LiteralConstant::Table::ALBUM)
                             .arg(LiteralConstant::Column::ALBUM_NAME);

        const auto appendSql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                              .arg(LiteralConstant::Table::ALBUM_MUSIC)
                              .arg(LiteralConstant::Column::ALBUM_ID)
                              .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&appendStmt, appendSql.toUtf8());
        core->stmtPrepare(&getIdStmt, getIdSql.toUtf8());

        for (const auto &data: dataList) {
            core->stmtReset(getIdStmt);
            core->stmtBindText(getIdStmt, 1, data.url);
            core->stmtBindText(getIdStmt, 2, data.album);
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

bool AlbumRepository::update(const AlbumPtr &album) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        if (album == nullptr)
            throw DataException("指针错误");

        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::ALBUM_NAME)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::ALBUM_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtReset(stmt);
        core->stmtBindText(stmt, 1, album->name);
        core->stmtBindText(stmt, 2, album->nameKey);
        core->stmtBindInt(stmt, 3, album->id);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

bool AlbumRepository::updateMusic(const QList<int> &musicIdList, const int albumNewId, const int albumOldId) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString(
                             "UPDATE %1 SET %2 = ? "
                             "WHERE %3 = ? AND %2 = ? "
                             "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                        .arg(LiteralConstant::Table::ALBUM_MUSIC)
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, albumNewId);
            core->stmtBindInt(stmt, 2, i);
            core->stmtBindInt(stmt, 3, albumOldId);
            core->stmtBindInt(stmt, 4, i);
            core->stmtBindInt(stmt, 5, albumNewId);
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

bool AlbumRepository::updateNameKey(const QStringList &albumName, const QStringList &albumNameKey) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                        .arg(LiteralConstant::Table::ALBUM)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::ALBUM_NAME);
        core->stmtPrepare(&stmt, sql.toUtf8());

        auto name = albumName.cbegin();
        auto key  = albumNameKey.cbegin();
        while (name != albumName.cend() && key != albumNameKey.cend()) {
            core->stmtReset(stmt);
            core->stmtBindText(stmt, 1, *key);
            core->stmtBindText(stmt, 2, *name);
            core->stmtStep(stmt);
            ++name;
            ++key;
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

bool AlbumRepository::moveMusic(const QString &albumName, const QString &albumNameNew) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    int           oldId  = -1;
    int           newId  = -1;

    try {
        core->begin();

        const auto getAlbumIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                  .arg(LiteralConstant::Column::ALBUM_ID)
                                  .arg(LiteralConstant::Table::ALBUM)
                                  .arg(LiteralConstant::Column::ALBUM_NAME);

        core->stmtPrepare(&stmt, getAlbumIdSql.toUtf8());
        core->stmtBindText(stmt, 1, albumName);
        if (core->stmtStep(stmt)) {
            oldId = sqlite3_column_int(stmt, 0);
        }
        core->stmtReset(stmt);

        core->stmtBindText(stmt, 1, albumNameNew);
        if (core->stmtStep(stmt)) {
            newId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (oldId == -1) {
            throw DataException("旧专辑不存在");
        }

        if (newId != -1) {
            const auto updateSql = QString("UPDATE %1 SET %2 = ? WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::ALBUM_MUSIC)
                                  .arg(LiteralConstant::Column::ALBUM_ID);
            core->stmtPrepare(&stmt, updateSql.toUtf8());
            core->stmtBindInt(stmt, 1, newId);
            core->stmtBindInt(stmt, 2, oldId);
            core->stmtStep(stmt);
            Core::stmtFree(stmt);
            stmt = nullptr;

            const auto deleteSql = QString("DELETE FROM %1 WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::ALBUM)
                                  .arg(LiteralConstant::Column::ALBUM_ID);
            core->stmtPrepare(&stmt, deleteSql.toUtf8());
            core->stmtBindInt(stmt, 1, oldId);
            core->stmtStep(stmt);
        } else {
            NameKey    key;
            const auto updateNameSql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                                      .arg(LiteralConstant::Table::ALBUM)
                                      .arg(LiteralConstant::Column::ALBUM_NAME)
                                      .arg(LiteralConstant::Column::NAME_KEY)
                                      .arg(LiteralConstant::Column::ALBUM_ID);
            core->stmtPrepare(&stmt, updateNameSql.toUtf8());
            core->stmtBindText(stmt, 1, albumNameNew);
            core->stmtBindText(stmt, 2, key.find(albumNameNew));
            core->stmtBindInt(stmt, 3, oldId);
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

bool AlbumRepository::addMusicToPlayList(const QString &albumName, const QString &playListName) const {
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
