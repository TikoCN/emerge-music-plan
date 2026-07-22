#include "delete.h"
#include "baseclass/DataException.h"
#include "Tlog.h"

bool Delete::deletePlayingList(const int position) const {
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

    return result;
}

bool Delete::clearInvalidData() const {
    return true;
}

QList<QPair<int, QString>> Delete::getAllMusicData() const {
    QList<QPair<int, QString>> musicDataList;

    const auto getAllMusicSql = QString("SELECT %1, %2 FROM %3")
                               .arg(LiteralConstant::Column::MUSIC_ID)
                               .arg(LiteralConstant::Column::URL)
                               .arg(LiteralConstant::Table::MUSIC);

    const sqlite3_callback collectCallback = [](void *data, int argc, char **argv, char **azColName) -> int {
        if (data == nullptr || argc < 2) {
            return SQLITE_OK;
        }

        auto *list = static_cast<QList<QPair<int, QString>> *>(data);
        int musicId = argv[0] ? QString(argv[0]).toInt() : 0;
        QString url = argv[1] ? QString(argv[1]) : QString();

        if (musicId > 0) {
            list->append(QPair<int, QString>(musicId, url));
        }

        return SQLITE_OK;
    };

    try {
        core->sqlExecuteCallBack(getAllMusicSql.toUtf8(), collectCallback, &musicDataList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }

    return musicDataList;
}

bool Delete::clearInvalidData(const QList<int> &invalidMusicIds) const {
    bool result = true;

    if (invalidMusicIds.isEmpty()) {
        return result;
    }

    try {
        core->begin();

        QString placeholders;
        for (int i = 0; i < invalidMusicIds.size(); ++i) {
            if (i > 0) placeholders += ",";
            placeholders += "?";
        }

        sqlite3_stmt *stmt = nullptr;

        const auto deleteMusicSql = QString(
                                        "DELETE FROM %1 WHERE %2 IN (%3)")
                                    .arg(LiteralConstant::Table::MUSIC)
                                    .arg(LiteralConstant::Column::MUSIC_ID)
                                    .arg(placeholders);
        core->stmtPrepare(&stmt, deleteMusicSql.toUtf8());
        for (int i = 0; i < invalidMusicIds.size(); ++i) {
            core->stmtBindInt(stmt, i + 1, invalidMusicIds[i]);
        }
        core->stmtStep(stmt);
        Core::stmtFree(stmt);
        stmt = nullptr;

        const auto deleteEmptyArtistSql = QString(
                                              "DELETE FROM %1 WHERE NOT EXISTS "
                                              "(SELECT 1 FROM %2 WHERE %3 = %1.%4)")
                                          .arg(LiteralConstant::Table::ARTIST)
                                          .arg(LiteralConstant::Table::ARTIST_MUSIC)
                                          .arg(LiteralConstant::Column::ARTIST_ID)
                                          .arg(LiteralConstant::Column::ARTIST_ID);
        core->sqlExecute(deleteEmptyArtistSql.toUtf8(), "删除空歌手失败");

        const auto deleteEmptyAlbumSql = QString(
                                               "DELETE FROM %1 WHERE NOT EXISTS "
                                               "(SELECT 1 FROM %2 WHERE %3 = %1.%4)")
                                           .arg(LiteralConstant::Table::ALBUM)
                                           .arg(LiteralConstant::Table::ALBUM_MUSIC)
                                           .arg(LiteralConstant::Column::ALBUM_ID)
                                           .arg(LiteralConstant::Column::ALBUM_ID);
        core->sqlExecute(deleteEmptyAlbumSql.toUtf8(), "删除空专辑失败");

        const auto deleteEmptyPlayListSql = QString(
                                                    "DELETE FROM %1 WHERE %2 = 0 AND NOT EXISTS "
                                                    "(SELECT 1 FROM %3 WHERE %4 = %1.%5)")
                                                .arg(LiteralConstant::Table::PLAYLIST)
                                                .arg(LiteralConstant::Column::IS_DIR)
                                                .arg(LiteralConstant::Table::PLAYLIST_MUSIC)
                                                .arg(LiteralConstant::Column::PLAYLIST_ID)
                                                .arg(LiteralConstant::Column::PLAYLIST_ID);
        core->sqlExecute(deleteEmptyPlayListSql.toUtf8(), "删除空播放列表失败");

        core->commit();
    } catch (const DataException &e) {
        core->rollback();
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }

    return result;
}
