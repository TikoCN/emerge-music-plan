#include "update.h"

bool Update::updateMusic(Music *music)
{
    QList<Music *> list;
    list.append(music);
    return updateMusic(list);
}

bool Update::updateMusic(QList<Music *> musicList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "music(music_id, title, album_id, duration, insert_time, level, love, play_number, url)"
                          "VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < musicList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            Music *music = musicList[i];
            r = sqlite3_bind_int(stmt, 1, music->coreId);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_text(stmt, 2, music->title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_bind_int(stmt, 3, music->albumId);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量3失败");

            r = sqlite3_bind_int(stmt, 4, music->endTime);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量4失败");

            r = sqlite3_bind_int(stmt, 5, music->lastEditTime);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量5失败");

            r = sqlite3_bind_int(stmt, 6, music->level);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量6失败");

            r = sqlite3_bind_int(stmt, 7, music->isLove);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量7失败");

            r = sqlite3_bind_int(stmt, 8, music->playNumber);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量8失败");

            r = sqlite3_bind_text(stmt, 9, music->url.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量9失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入歌曲数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateAlbum(Album *album)
{
    QList<Album *> list;
    list.append(album);
    return updateAlbum(list);
}

bool Update::updateAlbum(QList<Album *> albumList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "album(album_id, name)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < albumList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            Album *album = albumList[i];
            r = sqlite3_bind_int(stmt, 1, album->id);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_text(stmt, 2, album->name.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入专辑数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateArtist(Artist *artist)
{
    QList<Artist *> list;
    list.append(artist);
    return updateArtist(list);
}

bool Update::updateArtist(QList<Artist *> artistList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "artist(artist_id, name)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < artistList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            Artist *artist = artistList[i];
            r = sqlite3_bind_int(stmt, 1, artist->id);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_text(stmt, 2, artist->name.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入作曲家数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateTable(Table *table)
{
    QList<Table *> list;
    list.append(table);
    return updateTable(list);
}

bool Update::updateTable(QList<Table *> tableList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "playlist(list_id, name, sort, is_dir, url)"
                          "VALUES(?, ?, ?, ?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < tableList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            Table *table = tableList[i];
            r = sqlite3_bind_int(stmt, 1, table->tableId);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_text(stmt, 2, table->name.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_bind_int(stmt, 3, table->sort);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量3失败");

            r = sqlite3_bind_int(stmt, 4, table->isDir);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量4失败");

            r = sqlite3_bind_text(stmt, 5, table->url.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量5失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入列表数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateTableMusic(QPair<int, int> pair)
{
    QList<QPair<int, int> > list;
    list.append(pair);
    return updateTableMusic(list);
}

bool Update::updateTableMusic(QList<QPair<int, int> > pairList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "playlist_music(list_id, music_id)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < pairList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            QPair<int, int> pair = pairList[i];
            r = sqlite3_bind_int(stmt, 1, pair.first);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_int(stmt, 2, pair.second);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入列表-歌曲数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateTableMusic(Table *table)
{
    QList<Table *> list;
    list.append(table);
    return updateTableMusic(list);
}

bool Update::updateTableMusic(QList<Table *> tableList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "playlist_music(list_id, music_id)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < tableList.size(); ++i) {
            Table *table = tableList[i];
            for (int j = 0; j < table->musicList.size(); ++j) {
                r = sqlite3_reset(stmt); // 删除绑定
                if (r != SQLITE_OK) logError("重置 stmt 状态失败");
                r = sqlite3_bind_int(stmt, 1, table->tableId);
                if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

                r = sqlite3_bind_int(stmt, 2, table->musicList[j]);
                if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

                r = sqlite3_step(stmt);
                if (r != SQLITE_DONE) logError("插入列表-歌曲数据 SQLITE 语句执行失败");;
            }
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateArtistMusic(QPair<int, int> pair)
{
    QList<QPair<int, int> > list;
    list.append(pair);
    return updateArtistMusic(list);
}

bool Update::updateArtistMusic(QList<QPair<int, int> > pairList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "artist_music(artist_id, music_id)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < pairList.size(); ++i) {
            r = sqlite3_reset(stmt); // 删除绑定
            if (r != SQLITE_OK) logError("重置 stmt 状态失败");

            QPair<int, int> pair = pairList[i];
            r = sqlite3_bind_int(stmt, 1, pair.first);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

            r = sqlite3_bind_int(stmt, 2, pair.second);
            if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

            r = sqlite3_step(stmt);
            if (r != SQLITE_DONE) logError("插入作曲家-歌曲数据 SQLITE 语句执行失败");;
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

bool Update::updateArtistMusic(Artist *artist)
{
    QList<Artist *> list;
    list.append(artist);
    return updateArtistMusic(list);
}

bool Update::updateArtistMusic(QList<Artist *> artistList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR REPLACE INTO "
                          "artist_music(artist_id, music_id)"
                          "VALUES(?, ?)";
        r = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
        if (r != SQLITE_OK) logError("解析SQLITE语句失败");

        for (int i = 0; i < artistList.size(); ++i) {
            Artist *artist = artistList[i];
            for (int j = 0; j < artist->musicList.size(); ++j) {
                r = sqlite3_reset(stmt); // 删除绑定
                if (r != SQLITE_OK) logError("重置 stmt 状态失败");

                r = sqlite3_bind_int(stmt, 1, artist->id);
                if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

                r = sqlite3_bind_int(stmt, 2, artist->musicList[j]);
                if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量2失败");

                r = sqlite3_step(stmt);
                if (r != SQLITE_DONE) logError("插入作曲家-歌曲数据 SQLITE 语句执行失败");;
            }
        }

    } catch (QString e) {
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}
