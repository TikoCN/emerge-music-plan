#include "update.h"

bool Update::updateMusic(MusicPtr music)
{
    if (music == nullptr)
        return false;
    QList<MusicPtr> list = {music};
    return updateMusic(list);
}

bool Update::updateMusic(QList<MusicPtr> musicList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "UPDATE music "
                          "SET title = ?, album_id = ?, duration = ?, insert_time = ?, level = ?, love = ?, "
                          "play_number = ?, url = ?"
                          "WHERE music_id = ?";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicList.size(); ++i) {
            MusicPtr music = musicList[i];
            if (music == nullptr)
                continue;
            stmtReset(stmt);
            stmtBindText(stmt, 1, music->title);
            stmtBindInt(stmt, 2, music->albumId);
            stmtBindInt(stmt, 3, music->duration);
            stmtBindInt(stmt, 4, music->insetTime);
            stmtBindInt(stmt, 5, music->level);
            stmtBindInt(stmt, 6, music->isLove);
            stmtBindInt(stmt, 7, music->playNumber);
            stmtBindText(stmt, 8, music->url);
            stmtBindInt(stmt, 9, music->id);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Update::updatePlayList(PlayListPtr playlist)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        if (playlist == nullptr)
            throw QString("指针错误");

        const char *sql = "UPDATE playlist "
                          "SET name = ?, sort = ? "
                          "WHERE list_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, playlist->name);
        stmtBindInt(stmt, 2, (int)playlist->sort);
        stmtBindInt(stmt, 3, playlist->id);
        stmtStep(stmt);
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Update::updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId)
{
    QList<int> list = {musicId};
    return updatePlayListMusic(list, playlistNewId, playlistOldId);
}

bool Update::updatePlayListMusic(QList<int> musicIdList, int newId, int oldId)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "UPDATE playlist_music "
                          "SET list_id = ? "
                          "WHERE music_id = ? AND list_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND list_id = ? )";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicIdList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, newId);
            stmtBindInt(stmt, 2, musicIdList[i]);
            stmtBindInt(stmt, 3, oldId);
            stmtBindInt(stmt, 4, musicIdList[i]);
            stmtBindInt(stmt, 5, newId);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Update::updateArtist(ArtistPtr artist)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        if (artist == nullptr)
            throw QString("指针错误");

        const char *sql = "UPDATE artist "
                          "SET name = ?, key = ? "
                          "WHERE artist_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, artist->name);
        stmtBindText(stmt, 2, artist->lineKey);
        stmtBindInt(stmt, 3, artist->id);
        stmtStep(stmt);
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Update::updateArtistMusic(int musicId, int artistNewId, int artistOldId)
{
    QList<int> list = {musicId};
    return updateArtistMusic(list, artistNewId, artistOldId);
}

bool Update::updateArtistMusic(QList<int> musicIdList, int newId, int oldId)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "UPDATE artist_music "
                          "SET artist_id = ? "
                          "WHERE music_id = ? AND artist_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND artist_id = ? )";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicIdList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, newId);
            stmtBindInt(stmt, 2, musicIdList[i]);
            stmtBindInt(stmt, 3, oldId);
            stmtBindInt(stmt, 4, musicIdList[i]);
            stmtBindInt(stmt, 5, newId);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Update::updateAlbum(AlbumPtr album)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        if (album == nullptr)
            throw QString("指针错误");

        const char *sql = "UPDATE album "
                          "SET name = ?, key = ? "
                          "WHERE album_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, album->name);
        stmtBindText(stmt, 2, album->lineKey);
        stmtBindInt(stmt, 3, album->id);
        stmtStep(stmt);
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Update::updateAlbumMusic(int musicId, int albumNewId, int albumOldId)
{
    QList<int> list = {musicId};
    return updateAlbumMusic(list, albumNewId, albumOldId);
}

bool Update::updateAlbumMusic(QList<int> musicIdList, int newId, int oldId)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "UPDATE music "
                          "SET album_id = ? "
                          "WHERE music_id = ? AND album_id = ? AND ("
                          "SELECT 1 FROM playlist_music "
                          "WHERE music_id = ? AND album_id = ? )";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicIdList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, newId);
            stmtBindInt(stmt, 2, musicIdList[i]);
            stmtBindInt(stmt, 3, oldId);
            stmtBindInt(stmt, 4, musicIdList[i]);
            stmtBindInt(stmt, 5, newId);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}
