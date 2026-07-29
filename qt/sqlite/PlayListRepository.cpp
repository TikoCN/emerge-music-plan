#include "PlayListRepository.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileInfo>
#include "baseclass/DataException.h"
#include "baseclass/SortType.h"

PlayListPtr PlayListRepository::get(const int id) const {
    sqlite3_stmt *stmt     = nullptr;
    PlayListPtr   playList = nullptr;

    try {
        const auto sql = QString(
                             "SELECT %1.%2, %1.%3, %1.%4,"
                             "COUNT(DISTINCT %5.%6) AS music_count, SUM(%7.%8), "
                             "MIN(%7.%9) AS first_music_id "
                             "FROM %1 "
                             "LEFT JOIN %5 ON %1.%3 = %5.%10 "
                             "LEFT JOIN %7 ON %5.%6 = %7.%11 "
                             "WHERE %1.%3 = ? "
                             "GROUP BY %1.%3, %1.%2, %1.%4 "
                             "LIMIT 1")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Column::IS_DIR)
                        .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::MUSIC)
                        .arg(LiteralConstant::Column::DURATION)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, id);
        core->stmtStep(stmt);
        playList             = PlayListPtr(new PlayList());
        playList->name       = QString::fromUtf8(sqlite3_column_text(stmt, 0));
        playList->id         = sqlite3_column_int(stmt, 1);
        playList->isDir      = sqlite3_column_int(stmt, 2) == 1;
        playList->musicCount = sqlite3_column_int(stmt, 3);
        playList->duration   = sqlite3_column_int64(stmt, 4);
        playList->firstMusic = sqlite3_column_int(stmt, 5);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        playList = nullptr;
    }
    Core::stmtFree(stmt);
    return playList;
}

QList<int> PlayListRepository::getList(const int start, const int size, const bool isDir) const {
    QList<int>    list;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::IS_DIR);

        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, isDir ? 1 : 0);
        core->stmtBindInt(stmt, 2, size);
        core->stmtBindInt(stmt, 3, start);

        while (core->stmtStep(stmt)) {
            list.append(sqlite3_column_int(stmt, 0));
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        list.clear();
    }
    Core::stmtFree(stmt);
    return list;
}

QList<int> PlayListRepository::getMusic(const int id, const int size, const int start, const int sort) const {
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
                 .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                 .arg(LiteralConstant::Column::PLAYLIST_ID)
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
                 .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                 .arg(LiteralConstant::Column::PLAYLIST_ID)
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

QList<int> PlayListRepository::getMusicAll(const int id, const int sort) const {
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
                 .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                 .arg(LiteralConstant::Column::PLAYLIST_ID)
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
                 .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                 .arg(LiteralConstant::Column::PLAYLIST_ID)
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

int PlayListRepository::allowName(const QString &name) const {
    sqlite3_stmt *stmt = nullptr;
    int           r    = -1;
    try {
        const auto sql = QString("SELECT COALESCE("
                             "(SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1), "
                             "-1) AS %1")
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindText(stmt, 1, name);
        core->stmtStep(stmt);
        r = sqlite3_column_int(stmt, 0);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    // 存在返回0，不存在返回1
    return r >= 0 ? 0 : 1;
}

bool PlayListRepository::update(const PlayListPtr &playList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        if (playList == nullptr)
            throw DataException("指针错误");

        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::IS_DIR)
                        .arg(LiteralConstant::Column::PLAYLIST_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtReset(stmt);
        core->stmtBindText(stmt, 1, playList->name);
        core->stmtBindInt(stmt, 2, playList->isDir);
        core->stmtBindInt(stmt, 3, playList->id);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

QList<int> PlayListRepository::getPlayingListMusic() const {
    QList<int> idList;
    try {
        const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 ASC")
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::POSITION);
        core->sqlExecuteCallBack(sql.toUtf8(), Core::idListCallBack, &idList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return {};
    }
    return idList;
}

bool PlayListRepository::appendDir(const QStringList &urlList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();

        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::IS_DIR)
                        .arg(LiteralConstant::Column::SORT);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const QString &url: urlList) {
            core->stmtReset(stmt);
            core->stmtBindText(stmt, 1, QFileInfo(url).fileName());
            core->stmtBindText(stmt, 2, url);
            core->stmtBindInt(stmt, 3, 1);
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

bool PlayListRepository::appendUser(const QString &name) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3, %4, %5) VALUES(?, ?, ?, ?)")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::IS_DIR)
                        .arg(LiteralConstant::Column::SORT);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindText(stmt, 1, name);
        core->stmtBindText(stmt, 2, QString());
        core->stmtBindInt(stmt, 3, 0);
        core->stmtBindInt(stmt, 4, 1);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

bool PlayListRepository::appendPlayingMusic(const QList<int> &musicList, int start) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();

        const auto sql = QString("INSERT OR IGNORE INTO %1(%2, %3) VALUES(?, ?)")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, sql.toUtf8());
        int pos = start;
        for (const int id: musicList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, id);
            core->stmtBindInt(stmt, 2, pos++);
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

bool PlayListRepository::updateMusic(const QList<int> &musicIdList, const int playlistNewId, const int playlistOldId) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString(
                             "UPDATE %1 SET %2 = ? "
                             "WHERE %3 = ? AND %2 = ? "
                             "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                        .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                        .arg(LiteralConstant::Column::PLAYLIST_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, playlistNewId);
            core->stmtBindInt(stmt, 2, i);
            core->stmtBindInt(stmt, 3, playlistOldId);
            core->stmtBindInt(stmt, 4, i);
            core->stmtBindInt(stmt, 5, playlistNewId);
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

bool PlayListRepository::updatePlayingMusic(const QList<int> &musicIdList, int start) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();

        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::POSITION)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        int pos = start;
        for (const int id: musicIdList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, pos++);
            core->stmtBindInt(stmt, 2, id);
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

bool PlayListRepository::moveMusic(const QString &playListName, const QString &playListNameNew) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    int           oldId  = -1;
    int           newId  = -1;

    try {
        core->begin();

        const auto getPlayListIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                     .arg(LiteralConstant::Column::PLAYLIST_ID)
                                     .arg(LiteralConstant::Table::PLAYLIST)
                                     .arg(LiteralConstant::Column::PLAYLIST_NAME);

        core->stmtPrepare(&stmt, getPlayListIdSql.toUtf8());
        core->stmtBindText(stmt, 1, playListName);
        if (core->stmtStep(stmt)) {
            oldId = sqlite3_column_int(stmt, 0);
        }
        core->stmtReset(stmt);

        core->stmtBindText(stmt, 1, playListNameNew);
        if (core->stmtStep(stmt)) {
            newId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (oldId == -1) {
            throw DataException("旧播放列表不存在");
        }

        if (newId != -1) {
            const auto updateSql = QString("UPDATE %1 SET %2 = ? WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                                  .arg(LiteralConstant::Column::PLAYLIST_ID);
            core->stmtPrepare(&stmt, updateSql.toUtf8());
            core->stmtBindInt(stmt, 1, newId);
            core->stmtBindInt(stmt, 2, oldId);
            core->stmtStep(stmt);
            Core::stmtFree(stmt);
            stmt = nullptr;

            const auto deleteSql = QString("DELETE FROM %1 WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::PLAYLIST)
                                  .arg(LiteralConstant::Column::PLAYLIST_ID);
            core->stmtPrepare(&stmt, deleteSql.toUtf8());
            core->stmtBindInt(stmt, 1, oldId);
            core->stmtStep(stmt);
        } else {
            const auto updateNameSql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                                      .arg(LiteralConstant::Table::PLAYLIST)
                                      .arg(LiteralConstant::Column::PLAYLIST_NAME)
                                      .arg(LiteralConstant::Column::PLAYLIST_ID);
            core->stmtPrepare(&stmt, updateNameSql.toUtf8());
            core->stmtBindText(stmt, 1, playListNameNew);
            core->stmtBindInt(stmt, 2, oldId);
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

bool PlayListRepository::addMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const {
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
        Core::stmtFree(stmt);
        stmt = nullptr;

        core->stmtPrepare(&stmt, getPlayListIdSql.toUtf8());
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

bool PlayListRepository::deletePlayingList(int position) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        const auto sql = QString("DELETE FROM %1 WHERE %2 >= ?")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::POSITION);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, position);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}
