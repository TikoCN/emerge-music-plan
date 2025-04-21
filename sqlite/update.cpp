#include "update.h"

bool Update::updateMusic(Music *music)
{
    if (music == nullptr)
        return false;
    QList<Music *> list = {music};
    return updateMusic(list);
}

bool Update::updateMusic(QList<Music *> musicList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "UPDATE music "
                          "SET title = ?, album_id = ?, duration = ?, insert_time = ?, level = ?, love = ?, "
                          "play_number = ?, url = ?"
                          "WHERE music_id = ?";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicList.size(); ++i) {
            Music *music = musicList[i];
            if (music == nullptr)
                continue;
            stmtReset(stmt);
            stmtBindText(stmt, 1, music->title);
            stmtBindInt(stmt, 2, music->albumId);
            stmtBindInt(stmt, 3, music->endTime);
            stmtBindInt(stmt, 4, music->insetTime);
            stmtBindInt(stmt, 5, music->level);
            stmtBindInt(stmt, 6, music->isLove);
            stmtBindInt(stmt, 7, music->playNumber);
            stmtBindText(stmt, 8, music->url);
            stmtBindInt(stmt, 9, music->coreId);
            stmtStep(stmt);
        }
    } catch (QString e) {
        return false;
    }
    return true;
}

bool Update::updateTable(Table *table)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        if (table == nullptr)
            throw QString("指针错误");

        const char *sql = "UPDATE playlist "
                          "SET name = ?, sort = ? "
                          "WHERE list_id = ?";
        stmtPrepare(&stmt, sql);
        stmtReset(stmt);
        stmtBindText(stmt, 1, table->name);
        stmtBindInt(stmt, 2, (int)table->sort);
        stmtBindInt(stmt, 3, table->tableId);
        stmtStep(stmt);
    } catch (QString e) {
        return false;
    }
    return true;
}

bool Update::updateTableMusic(int musicId, int tableNewId, int tableOldId)
{
    QList<int> list = {musicId};
    return updateTableMusic(list, tableNewId, tableOldId);
}

bool Update::updateTableMusic(QList<int> musicIdList, int newId, int oldId)
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
        return false;
    }
    return true;
}

bool Update::updateArtist(Artist *artist)
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
        return false;
    }
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
        return false;
    }
    return true;
}

bool Update::updateAlbum(Album *album)
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
        return false;
    }
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
        return false;
    }
    return true;
}
