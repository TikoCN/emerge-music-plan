#include "Update.h"
#include "baseclass/DataException.h"

bool Update::updateMusic(const MusicPtr &music) {
    if (music == nullptr)
        return false;
    const QList<MusicPtr> list = {music};
    return updateMusic(list);
}

bool Update::updateMusic(const QList<MusicPtr> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // UPDATE music SET title = ?, duration = ?, level = ?, love = ?, play_number = ?, url = ? WHERE music_id = ?
        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ?, %4 = ?, %5 = ?, %6 = ?, %7 = ? WHERE %8 = ?")
                .arg(LiteralConstant::Table::MUSIC) // %1
                .arg(LiteralConstant::Column::TITLE) // %2
                .arg(LiteralConstant::Column::DURATION) // %3
                .arg(LiteralConstant::Column::LEVEL) // %4
                .arg(LiteralConstant::Column::IS_LOVE) // %5
                .arg(LiteralConstant::Column::PLAY_NUMBER) // %6
                .arg(LiteralConstant::Column::URL) // %7
                .arg(LiteralConstant::Column::MUSIC_ID); // %8
        stmtPrepare(&stmt, sql.toUtf8());
        for (const auto &music: musicList) {
            if (music == nullptr)
                continue;
            stmtReset(stmt);
            stmtBindText(stmt, 1, music->title);
            stmtBindLong(stmt, 2, music->duration);
            stmtBindInt(stmt, 3, music->level);
            stmtBindInt(stmt, 4, music->isLove);
            stmtBindInt(stmt, 5, music->playNumber);
            stmtBindText(stmt, 6, music->url);
            stmtBindInt(stmt, 7, music->id);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Update::updatePlayList(const PlayListPtr &playlist) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (playlist == nullptr)
            throw DataException("指针错误");

        // UPDATE playlist SET name = ?, sort = ? WHERE list_id = ?
        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                .arg(LiteralConstant::Table::PLAYLIST) // %1
                .arg(LiteralConstant::Column::PLAYLIST_NAME) // %2 (name)
                .arg(LiteralConstant::Column::SORT) // %3 (sort)
                .arg(LiteralConstant::Column::PLAYLIST_ID); // %4 (list_id)
        stmtPrepare(&stmt, sql.toUtf8());
        stmtReset(stmt);
        stmtBindText(stmt, 1, playlist->name);
        stmtBindInt(stmt, 2, (int) playlist->sortType);
        stmtBindInt(stmt, 3, playlist->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updatePlayListMusic(const int musicId, const int playlistNewId, const int playlistOldId) {
    const QList<int> list = {musicId};
    return updatePlayListMusic(list, playlistNewId, playlistOldId);
}

bool Update::updatePlayingListMusic(const int musicId, int position) {
    const QList<int> list = {musicId};
    return updatePlayingListMusic(list, position);
}

bool Update::updatePlayingListMusic(const QList<int> &musicIdList, int start) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // UPDATE playinglist SET position = ? WHERE music_id = ?
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                .arg(LiteralConstant::Table::PLAYINGLIST) // %1
                .arg(LiteralConstant::Column::POSITION) // %2 (position)
                .arg(LiteralConstant::Column::MUSIC_ID); // %3 (music_id)
        stmtPrepare(&stmt, sql.toUtf8());

        for (int i = 0; i < musicIdList.size(); i++) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, start + i);
            stmtBindInt(stmt, 2, musicIdList[i]);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtist(const ArtistPtr &artist) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (artist == nullptr)
            throw DataException("指针错误");

        // UPDATE artist SET name = ?, key = ? WHERE artist_id = ?
        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                .arg(LiteralConstant::Table::ARTIST) // %1
                .arg(LiteralConstant::Column::ARTIST_NAME) // %2 (name)
                .arg(LiteralConstant::Column::NAME_KEY) // %3 (key)
                .arg(LiteralConstant::Column::ARTIST_ID); // %4 (artist_id)
        stmtPrepare(&stmt, sql.toUtf8());
        stmtReset(stmt);
        stmtBindText(stmt, 1, artist->name);
        stmtBindText(stmt, 2, artist->lineKey);
        stmtBindInt(stmt, 3, artist->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtistMusic(const int musicId, const int artistNewId, const int artistOldId) {
    const QList<int> list = {musicId};
    return updateArtistMusic(list, artistNewId, artistOldId);
}

bool Update::updateAlbum(const AlbumPtr &album) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (album == nullptr)
            throw DataException("指针错误");

        // UPDATE album SET name = ?, key = ? WHERE album_id = ?
        const auto sql = QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %4 = ?")
                .arg(LiteralConstant::Table::ALBUM) // %1
                .arg(LiteralConstant::Column::ALBUM_NAME) // %2 (name)
                .arg(LiteralConstant::Column::NAME_KEY) // %3 (key)
                .arg(LiteralConstant::Column::ALBUM_ID); // %4 (album_id)
        stmtPrepare(&stmt, sql.toUtf8());
        stmtReset(stmt);
        stmtBindText(stmt, 1, album->name);
        stmtBindText(stmt, 2, album->lineKey);
        stmtBindInt(stmt, 3, album->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateAlbumMusic(const int musicId, const int albumNewId, const int albumOldId) {
    const QList<int> list = {musicId};
    return updateAlbumMusic(list, albumNewId, albumOldId);
}

bool Update::updateAlbumNameKey(const QStringList &albumName, const QStringList &albumNameKey) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // UPDATE album SET key = ? WHERE name = ?
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                .arg(LiteralConstant::Table::ALBUM) // %1
                .arg(LiteralConstant::Column::NAME_KEY) // %2 (key)
                .arg(LiteralConstant::Column::ALBUM_NAME); // %3 (name)
        stmtPrepare(&stmt, sql.toUtf8());

        auto name = albumName.cbegin();
        auto key = albumNameKey.cbegin();
        while (name != albumName.cend() && key != albumNameKey.cend()) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, *key);
            stmtBindText(stmt, 2, *name);
            stmtStep(stmt);
            ++name;
            ++key;
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtistNameKey(const QStringList &artistName, const QStringList &artistNameKey) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // UPDATE artist SET key = ? WHERE name = ?
        const auto sql = QString("UPDATE %1 SET %2 = ? WHERE %3 = ?")
                .arg(LiteralConstant::Table::ARTIST) // %1
                .arg(LiteralConstant::Column::NAME_KEY) // %2 (key)
                .arg(LiteralConstant::Column::ARTIST_NAME); // %3 (name)
        stmtPrepare(&stmt, sql.toUtf8());

        auto name = artistName.cbegin();
        auto key = artistNameKey.cbegin();
        while (name != artistName.cend() && key != artistNameKey.cend()) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, *key);
            stmtBindText(stmt, 2, *name);
            stmtStep(stmt);
            ++name;
            ++key;
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updatePlayListMusic(const QList<int> &musicIdList, const int playlistNewId, const int playlistOldId) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 正确的 SQL：仅在目标播放列表中不存在该音乐时才更新
        const auto sql = QString(
                    "UPDATE %1 SET %2 = ? "
                    "WHERE %3 = ? AND %2 = ? "
                    "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                .arg(LiteralConstant::Table::PLAYLIST_MUSIC) // %1
                .arg(LiteralConstant::Column::PLAYLIST_ID) // %2 (list_id)
                .arg(LiteralConstant::Column::MUSIC_ID); // %3 (music_id)

        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, playlistNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, playlistOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, playlistNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtistMusic(const QList<int> &musicIdList, const int artistNewId, const int artistOldId) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 修正：使用正确的表名 artist_music，并添加重复检查
        const auto sql = QString(
                    "UPDATE %1 SET %2 = ? "
                    "WHERE %3 = ? AND %2 = ? "
                    "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                .arg(LiteralConstant::Table::ARTIST_MUSIC) // %1
                .arg(LiteralConstant::Column::ARTIST_ID) // %2
                .arg(LiteralConstant::Column::MUSIC_ID); // %3

        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, artistNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, artistOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, artistNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateAlbumMusic(const QList<int> &musicIdList, const int albumNewId, const int albumOldId) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        // 修正：应更新 album_music 表，而非 music 表
        const auto sql = QString(
                    "UPDATE %1 SET %2 = ? "
                    "WHERE %3 = ? AND %2 = ? "
                    "AND NOT EXISTS (SELECT 1 FROM %1 WHERE %3 = ? AND %2 = ?)")
                .arg(LiteralConstant::Table::ALBUM_MUSIC) // %1
                .arg(LiteralConstant::Column::ALBUM_ID) // %2
                .arg(LiteralConstant::Column::MUSIC_ID); // %3

        stmtPrepare(&stmt, sql.toUtf8());
        for (const int i: musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, albumNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, albumOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, albumNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}