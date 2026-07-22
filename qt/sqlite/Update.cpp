#include "Update.h"
#include "baseclass/DataException.h"
#include "../NameKey.h"

bool Update::updateMusic(const MusicPtr &music) const {
    if (music == nullptr)
        return false;
    const QList<MusicPtr> list = {music};
    return updateMusic(list);
}

bool Update::updateMusic(const QList<MusicPtr> &musicList) const {
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

bool Update::updatePlayList(const PlayListPtr &playlist) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        if (playlist == nullptr)
            throw DataException("指针错误");

        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                        .arg(LiteralConstant::Table::PLAYLIST)
                        .arg(LiteralConstant::Column::PLAYLIST_NAME)
                        .arg(LiteralConstant::Column::SORT)
                        .arg(LiteralConstant::Column::PLAYLIST_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtReset(stmt);
        core->stmtBindText(stmt, 1, playlist->name);
        core->stmtBindInt(stmt, 2, (int) playlist->sort);
        core->stmtBindInt(stmt, 3, playlist->id);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

bool Update::updatePlayListMusic(const int musicId, const int playlistNewId, const int playlistOldId) const {
    const QList<int> list = {musicId};
    return updatePlayListMusic(list, playlistNewId, playlistOldId);
}

bool Update::updatePlayingListMusic(const int musicId, int position) const {
    const QList<int> list = {musicId};
    return updatePlayingListMusic(list, position);
}

bool Update::updatePlayingListMusic(const QList<int> &musicIdList, int start) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                        .arg(LiteralConstant::Table::PLAYINGLIST)
                        .arg(LiteralConstant::Column::POSITION)
                        .arg(LiteralConstant::Column::MUSIC_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());

        for (int i = 0; i < musicIdList.size(); i++) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, start + i);
            core->stmtBindInt(stmt, 2, musicIdList[i]);
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

bool Update::updateArtist(const ArtistPtr &artist) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        if (artist == nullptr)
            throw DataException("指针错误");

        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                        .arg(LiteralConstant::Table::ARTIST)
                        .arg(LiteralConstant::Column::ARTIST_NAME)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::ARTIST_ID);
        core->stmtPrepare(&stmt, sql.toUtf8());
        core->stmtReset(stmt);
        core->stmtBindText(stmt, 1, artist->name);
        core->stmtBindText(stmt, 2, artist->nameKey);
        core->stmtBindInt(stmt, 3, artist->id);
        core->stmtStep(stmt);
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        result = false;
    }
    Core::stmtFree(stmt);
    return result;
}

bool Update::updateArtistMusic(const int musicId, const int artistNewId, const int artistOldId) const {
    const QList<int> list = {musicId};
    return updateArtistMusic(list, artistNewId, artistOldId);
}

bool Update::updateAlbum(const AlbumPtr &album) const {
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

bool Update::updateAlbumMusic(const int musicId, const int albumNewId, const int albumOldId) const {
    const QList<int> list = {musicId};
    return updateAlbumMusic(list, albumNewId, albumOldId);
}

bool Update::updateAlbumNameKey(const QStringList &albumName, const QStringList &albumNameKey) const {
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

bool Update::updateArtistNameKey(const QStringList &artistName, const QStringList &artistNameKey) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                        .arg(LiteralConstant::Table::ARTIST)
                        .arg(LiteralConstant::Column::NAME_KEY)
                        .arg(LiteralConstant::Column::ARTIST_NAME);
        core->stmtPrepare(&stmt, sql.toUtf8());

        auto name = artistName.cbegin();
        auto key  = artistNameKey.cbegin();
        while (name != artistName.cend() && key != artistNameKey.cend()) {
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

bool Update::updatePlayListMusic(const QList<int> &musicIdList, const int playlistNewId, const int playlistOldId) const {
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

bool Update::updateArtistMusic(const QList<int> &musicIdList, const int artistNewId, const int artistOldId) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    try {
        core->begin();
        const auto sql = QString(
                             "UPDATE %1 SET %2 = ? "
                             "WHERE %3 = ? AND %2 = ? "
                             "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                        .arg(LiteralConstant::Table::ARTIST_MUSIC)
                        .arg(LiteralConstant::Column::ARTIST_ID)
                        .arg(LiteralConstant::Column::MUSIC_ID);

        core->stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            core->stmtReset(stmt);
            core->stmtBindInt(stmt, 1, artistNewId);
            core->stmtBindInt(stmt, 2, i);
            core->stmtBindInt(stmt, 3, artistOldId);
            core->stmtBindInt(stmt, 4, i);
            core->stmtBindInt(stmt, 5, artistNewId);
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

bool Update::updateAlbumMusic(const QList<int> &musicIdList, const int albumNewId, const int albumOldId) const {
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

bool Update::moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const {
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

bool Update::moveArtistMusic(const QString &artistName, const QString &artistNameNew) const {
    bool          result = true;
    sqlite3_stmt *stmt   = nullptr;
    int           oldId  = -1;
    int           newId  = -1;

    try {
        core->begin();

        const auto getArtistIdSql = QString("SELECT %1 FROM %2 WHERE %3 = ? LIMIT 1")
                                   .arg(LiteralConstant::Column::ARTIST_ID)
                                   .arg(LiteralConstant::Table::ARTIST)
                                   .arg(LiteralConstant::Column::ARTIST_NAME);

        core->stmtPrepare(&stmt, getArtistIdSql.toUtf8());
        core->stmtBindText(stmt, 1, artistName);
        if (core->stmtStep(stmt)) {
            oldId = sqlite3_column_int(stmt, 0);
        }
        core->stmtReset(stmt);

        core->stmtBindText(stmt, 1, artistNameNew);
        if (core->stmtStep(stmt)) {
            newId = sqlite3_column_int(stmt, 0);
        }
        Core::stmtFree(stmt);
        stmt = nullptr;

        if (oldId == -1) {
            throw DataException("旧歌手不存在");
        }

        if (newId != -1) {
            const auto updateSql = QString(
                                       "UPDATE %1 SET %2 = ? "
                                       "WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::ARTIST_MUSIC)
                                  .arg(LiteralConstant::Column::ARTIST_ID);
            core->stmtPrepare(&stmt, updateSql.toUtf8());
            core->stmtBindInt(stmt, 1, newId);
            core->stmtBindInt(stmt, 2, oldId);
            core->stmtStep(stmt);
            Core::stmtFree(stmt);
            stmt = nullptr;

            const auto deleteSql = QString("DELETE FROM %1 WHERE %2 = ?")
                                  .arg(LiteralConstant::Table::ARTIST)
                                  .arg(LiteralConstant::Column::ARTIST_ID);
            core->stmtPrepare(&stmt, deleteSql.toUtf8());
            core->stmtBindInt(stmt, 1, oldId);
            core->stmtStep(stmt);
        } else {
            NameKey    key;
            const auto updateNameSql = QString(
                                           "UPDATE %1 SET %2 = ?, %3 = ? "
                                           "WHERE %4 = ?")
                                      .arg(LiteralConstant::Table::ARTIST)
                                      .arg(LiteralConstant::Column::ARTIST_NAME)
                                      .arg(LiteralConstant::Column::NAME_KEY)
                                      .arg(LiteralConstant::Column::ARTIST_ID);
            core->stmtPrepare(&stmt, updateNameSql.toUtf8());
            core->stmtBindText(stmt, 1, artistNameNew);
            core->stmtBindText(stmt, 2, key.find(artistNameNew));
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

bool Update::movePlayListMusic(const QString &playListName, const QString &playListNameNew) const {
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
            const auto updateSql = QString(
                                      "UPDATE %1 SET %2 = ? "
                                      "WHERE %2 = ?")
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
            const auto updateNameSql = QString(
                                           "UPDATE %1 SET %2 = ? "
                                           "WHERE %3 = ?")
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
