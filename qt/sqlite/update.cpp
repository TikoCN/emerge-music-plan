#include "update.h"
#include "baseclass/dataexception.h"

bool Update::updateMusic(const MusicPtr &music)
{
    if (music == nullptr)
        return false;
    const QList<MusicPtr> list = {music};
    return updateMusic(list);
}

bool Update::updateMusic(const QList<MusicPtr>& musicList)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE music "
                          "SET title = ?, album_id = ?, duration = ?, level = ?, love = ?, "
                          "play_number = ?, url = ? "
                          "WHERE music_id = ?";
        stmtPrepare(&stmt, sql);
        for (const auto& music : musicList) {
            if (music == nullptr)
                continue;
            stmtReset(stmt);
            stmtBindText(stmt, 1, music->title);
            stmtBindInt(stmt, 2, music->albumId);
            stmtBindLong(stmt, 3, music->duration);
            stmtBindInt(stmt, 4, music->level);
            stmtBindInt(stmt, 5, music->isLove);
            stmtBindInt(stmt, 6, music->playNumber);
            stmtBindText(stmt, 7, music->url);
            stmtBindInt(stmt, 8, music->id);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Update::updatePlayList(const PlayListPtr& playlist)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (playlist == nullptr)
            throw DataException("指针错误");

        const auto sql = "UPDATE playlist "
                          "SET name = ?, sort = ? "
                          "WHERE list_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, playlist->name);
        stmtBindInt(stmt, 2, (int)playlist->sort);
        stmtBindInt(stmt, 3, playlist->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updatePlayListMusic(const int musicId, const int playlistNewId, const int playlistOldId)
{
    const QList<int> list = {musicId};
    return updatePlayListMusic(list, playlistNewId, playlistOldId);
}

bool Update::updatePlayListMusic(const QList<int>& musicIdList, const int playlistNewId, const int playlistOldId)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE playlist_music "
                          "SET list_id = ? "
                          "WHERE music_id = ? AND list_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND list_id = ? )";
        stmtPrepare(&stmt, sql);
        for (const int i : musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, playlistNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, playlistOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, playlistNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtist(const ArtistPtr& artist)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (artist == nullptr)
            throw DataException("指针错误");

        const auto sql = "UPDATE artist "
                          "SET name = ?, key = ? "
                          "WHERE artist_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, artist->name);
        stmtBindText(stmt, 2, artist->lineKey);
        stmtBindInt(stmt, 3, artist->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtistMusic(const int musicId, const int artistNewId, const int artistOldId)
{
    const QList<int> list = {musicId};
    return updateArtistMusic(list, artistNewId, artistOldId);
}

bool Update::updateArtistMusic(const QList<int>& musicIdList, const int artistNewId, const int artistOldId)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE artist_music "
                          "SET artist_id = ? "
                          "WHERE music_id = ? AND artist_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND artist_id = ? )";
        stmtPrepare(&stmt, sql);
        for (const int i : musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, artistNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, artistOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, artistNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateAlbum(const AlbumPtr& album)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        if (album == nullptr)
            throw DataException("指针错误");

        const auto sql = "UPDATE album "
                          "SET name = ?, key = ? "
                          "WHERE album_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, album->name);
        stmtBindText(stmt, 2, album->lineKey);
        stmtBindInt(stmt, 3, album->id);
        stmtStep(stmt);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateAlbumMusic(const int musicId, const int albumNewId, const int albumOldId)
{
    const QList<int> list = {musicId};
    return updateAlbumMusic(list, albumNewId, albumOldId);
}

bool Update::updateAlbumMusic(const QList<int>& musicIdList, const int albumNewId, const int albumOldId)
{
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE music "
                          "SET album_id = ? "
                          "WHERE music_id = ? AND album_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND album_id = ? )";
        stmtPrepare(&stmt, sql);
        for (const int i : musicIdList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, albumNewId);
            stmtBindInt(stmt, 2, i);
            stmtBindInt(stmt, 3, albumOldId);
            stmtBindInt(stmt, 4, i);
            stmtBindInt(stmt, 5, albumNewId);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateAlbumNameKey(const QStringList &albumName, const QStringList &albumNameKey) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE album "
                         "SET key = ? "
                         "WHERE name = ? ";
        stmtPrepare(&stmt, sql);

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
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Update::updateArtistNameKey(const QStringList &artistName, const QStringList &artistNameKey) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "UPDATE artist "
                         "SET key = ? "
                         "WHERE name = ? ";
        stmtPrepare(&stmt, sql);

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
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}
