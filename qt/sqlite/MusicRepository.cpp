#include "MusicRepository.h"
#include <QJsonDocument>
#include <QJsonObject>
#include "baseclass/DataException.h"
#include "NameKey.h"

QStringList MusicRepository::getMusicKeys() const {
    QStringList keyList;
    try {
        const auto sql = QString("SELECT DISTINCT %1 FROM %2 ORDER BY %1 ASC")
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Table::MUSIC);

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

QList<int> MusicRepository::getMusicByKey(const QString &key, const int size, const int start) const {
    QList<int>    list;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT ? OFFSET ?")
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::MUSIC)
                        .arg(LiteralConstant::Column::NAME_KEY);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindText(stmt, 1, key);
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

QString MusicRepository::getMusicUrl(const int id) const {
    sqlite3_stmt *stmt = nullptr;
    QString       url;
    try {
        const auto sql = QString("SELECT %1 FROM %2 WHERE %3=? LIMIT 1")
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Table::MUSIC)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtBindInt(stmt, 1, id);
        core->stmtStep(stmt);
        url = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    Core::stmtFree(stmt);
    return url;
}

MusicPtr MusicRepository::getMusic(const int id) const {
    QList<int> idList;
    idList.append(id);

    if (const QHash<int, MusicPtr> hash = getMusic(idList); hash.contains(id)) {
        return hash.value(id);
    }
    return nullptr;
}

QHash<int, MusicPtr> MusicRepository::getMusic(const QList<int> &idList) const {
    QHash<int, MusicPtr> hash;
    sqlite3_stmt *       stmt = nullptr;
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
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Column::TITLE)
                        .arg(LiteralConstant::Column::DURATION)
                        .arg(LiteralConstant::Column::LAST_EDIT_TIME)
                        .arg(LiteralConstant::Column::LEVEL)
                        .arg(LiteralConstant::Column::IS_LOVE)
                        .arg(LiteralConstant::Column::PLAY_NUMBER)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::ALBUM_NAME)
                        .arg(LiteralConstant::Column::ARTIST_NAME)
                        .arg(LiteralConstant::Column::ALBUM_ID)
                        .arg(LiteralConstant::Column::ARTIST_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());
        for (int i: idList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, i);
            core->stmtStep(stmt);
            MusicPtr music(new Music);

            music->id         = sqlite3_column_int(stmt, 0);
            music->title      = QString::fromUtf8(sqlite3_column_text(stmt, 1));
            music->duration   = sqlite3_column_int64(stmt, 2);
            music->insetTime  = sqlite3_column_int64(stmt, 3);
            music->level      = sqlite3_column_int(stmt, 4);
            music->isLove     = sqlite3_column_int(stmt, 5) == 1;
            music->playNumber = sqlite3_column_int(stmt, 6);
            music->url        = QString::fromUtf8(sqlite3_column_text(stmt, 7));
            music->album      = QString::fromUtf8(sqlite3_column_text(stmt, 8));
            music->artist     = QString::fromUtf8(sqlite3_column_text(stmt, 9));

            music->fromFileInfo(QFileInfo(music->url));
            hash.insert(i, music);
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        hash.clear();
    }
    Core::stmtFree(stmt);
    return hash;
}

QList<int> MusicRepository::getMusicRandList(int length) const {
    length = (length == -1) ? 15 : length;

    const QString sql = QString("SELECT %1 FROM %2 ORDER BY RANDOM() LIMIT %3")
                       .arg(LiteralConstant::Column::MUSIC_ID)
                       .arg(LiteralConstant::Table::MUSIC)
                       .arg(length);
    QList<int> idList;
    try {
        core->sqlExecuteCallBack(sql.toUtf8(), Core::idListCallBack, &idList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return {};
    }
    return idList;
}

QList<int> MusicRepository::getNewMusicList() const {
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 15")
                    .arg(LiteralConstant::Column::MUSIC_ID)
                    .arg(LiteralConstant::Table::MUSIC)
                    .arg(LiteralConstant::Column::PLAY_NUMBER);
    QList<int> idList;
    try {
        core->sqlExecuteCallBack(sql.toUtf8(), Core::idListCallBack, &idList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return {};
    }
    return idList;
}

QList<int> MusicRepository::getReadMoreList() const {
    const auto sql = QString("SELECT %1 FROM %2 ORDER BY %3 DESC LIMIT 15")
                    .arg(LiteralConstant::Column::MUSIC_ID)
                    .arg(LiteralConstant::Table::MUSIC)
                    .arg(LiteralConstant::Column::LAST_EDIT_TIME);
    QList<int> idList;
    try {
        core->sqlExecuteCallBack(sql.toUtf8(), Core::idListCallBack, &idList);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return {};
    }
    return idList;
}

bool MusicRepository::appendMusic(const MediaData &data) const {
    QList<MediaData> dataList;
    dataList.append(data);
    return appendMusic(dataList);
}

bool MusicRepository::appendMusic(const QList<MediaData> &dataList) const {
    bool          result          = true;
    sqlite3_stmt *appendMusicStmt = nullptr;
    sqlite3_stmt *getAlbumIDStmt  = nullptr;

    try {
        core->begin();

        const auto getAlbumIDSql = QString("SELECT %1 FROM %2 WHERE %3=? LIMIT 1")
                                  .arg(LiteralConstant::Column::ALBUM_ID)
                                  .arg(LiteralConstant::Table::ALBUM)
                                  .arg(LiteralConstant::Column::ALBUM_NAME);

        const auto appendMusicSql = QString(
                                        "INSERT OR IGNORE INTO %1(%2, %3, %4, %5, %6, %7, %8, %9) VALUES(?, ?, ?, ?, ?, ?, ?, ?)")
                                   .arg(LiteralConstant::Table::MUSIC)
                                   .arg(LiteralConstant::Column::TITLE)
                                   .arg(LiteralConstant::Column::DURATION)
                                   .arg(LiteralConstant::Column::LAST_EDIT_TIME)
                                   .arg(LiteralConstant::Column::LEVEL)
                                   .arg(LiteralConstant::Column::IS_LOVE)
                                   .arg(LiteralConstant::Column::PLAY_NUMBER)
                                   .arg(LiteralConstant::Column::URL)
                                   .arg(LiteralConstant::Column::NAME_KEY);

        core->stmtPrepare(&appendMusicStmt, appendMusicSql.toUtf8());
        core->stmtPrepare(&getAlbumIDStmt, getAlbumIDSql.toUtf8());

        NameKey key;

        for (const MediaData &data: dataList) {
            core->stmtReset(getAlbumIDStmt);
            core->stmtBindText(getAlbumIDStmt, 1, data.album);
            core->stmtStep(getAlbumIDStmt);

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

bool MusicRepository::updateMusic(const MusicPtr &music) const {
    if (music == nullptr)
        return false;
    const QList<MusicPtr> list = {music};
    return updateMusic(list);
}

bool MusicRepository::updateMusic(const QList<MusicPtr> &musicList) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ?, %4 = ?, %5 = ?, %6 = ?, %7 = ? WHERE %8 = ?")
                        .arg(LiteralConstant::Table::MUSIC)
                        .arg(LiteralConstant::Column::TITLE)
                        .arg(LiteralConstant::Column::DURATION)
                        .arg(LiteralConstant::Column::LEVEL)
                        .arg(LiteralConstant::Column::IS_LOVE)
                        .arg(LiteralConstant::Column::PLAY_NUMBER)
                        .arg(LiteralConstant::Column::URL)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const auto &music: musicList) {
            if (music == nullptr)
                continue;
            core->stmtReset(stmt);
            core->stmtBindText(stmt, 1, music->title);
            core->stmtBindLong(stmt, 2, music->duration);
            core->stmtBindInt(stmt, 3, music->level);
            core->stmtBindInt(stmt, 4, music->isLove);
            core->stmtBindInt(stmt, 5, music->playNumber);
            core->stmtBindText(stmt, 6, music->url);
            core->stmtBindInt(stmt, 7, music->id);
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

QString MusicRepository::getAllList() const {
    QString musicListStr;
    try {
        const auto sql = QString("SELECT GROUP_CONCAT(%1) FROM %2")
                        .arg(LiteralConstant::Column::MUSIC_ID)
                        .arg(LiteralConstant::Table::MUSIC);
        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName)-> int {
            auto *str = static_cast<QString *>(data);
            *str = QString(*argv);
            return SQLITE_OK;
        };
        core->sqlExecuteCallBack(sql.toUtf8(), callback, &musicListStr);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }
    return musicListStr;
}

QList<QPair<int, QString>> MusicRepository::getAllMusicData() const {
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

bool MusicRepository::clearInvalidData(const QList<int> &invalidMusicIds) const {
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