#include "sqlite.h"
#include <QDebug>

SQLite *SQLite::getInstance()
{
    return instance;
}

SQLite::SQLite() {
    error = nullptr;

    try{
        int flags =  SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI | SQLITE_OPEN_FULLMUTEX;
        r = sqlite3_open_v2("file:data.db", &db, flags, nullptr);
        if (r != SQLITE_OK) logError("打开数据库文件失败");
        int value = 0;

        // 启用外键约束
        const char* enableFK = "PRAGMA foreign_keys = ON;";
        r = sqlite3_exec(db, enableFK, nullptr, nullptr, nullptr);
        if (r != SQLITE_OK) logError("启用外键失败");

        const char *check = "SELECT count(*) FROM sqlite_master WHERE type='table' AND ("
                            "name='music' OR "
                            "name='playlist' OR "
                            "name='playlist_music' OR "
                            "name='artist' OR "
                            "name='artist_music' OR "
                            "name='album'"
                            ")";

        r = sqlite3_exec(db, check, countCallBack, &value, &error);
        if (r != SQLITE_OK) logError("查询数据库中表完整性失败");

        if (value == 6) return;
        else if (value > 0) logError("数据库异常，请删除 data.db 文件");

        // 检测 playlist
        {
            const char *sql = "CREATE TABLE playlist("
                              "list_id INT PRIMARY KEY NOT NULL,"
                              "name TEXT NOT NULL,"
                              "sort INT NOT NULL,"
                              "url TEXT,"
                              "is_dir INT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建 playlist 表失败");
        }

        // 检测 artist
        {
            const char *sql = "CREATE TABLE artist("
                              "artist_id INT PRIMARY KEY NOT NULL,"
                              "name TEXT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建artist表失败");
        }

        // 检测 album
        {
            const char *sql = "CREATE TABLE album("
                              "album_id INT PRIMARY KEY NOT NULL,"
                              "name TEXT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建album表失败");
        }

        // music
        {
            const char *sql = "CREATE TABLE music("
                              "music_id INT PRIMARY KEY NOT NULL,"
                              "title TEXT NOT NULL,"
                              "album_id INT NOT NULL,"
                              "duration INT NOT NULL,"
                              "insert_time INT NOT NULL,"
                              "level INT NOT NULL,"
                              "love INT NOT NULL,"
                              "play_number INT NOT NULL,"
                              "url TEXT NOT NULL,"
                              "FOREIGN KEY (album_id) REFERENCES album(album_id) ON DELETE CASCADE"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建music表失败");
        }

        // 检测 playlist_music
        {
            const char *sql = "CREATE TABLE playlist_music("
                              "music_id INT NOT NULL,"
                              "list_id INT NOT NULL,"
                              "PRIMARY KEY (music_id, list_id),"
                              "FOREIGN KEY (music_id) REFERENCES music(music_id) ON DELETE CASCADE,"
                              "FOREIGN KEY (list_id) REFERENCES playlist(list_id) ON DELETE CASCADE"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建playlist_music表失败");
        }

        // 检测 artist_music
        {
            const char *sql = "CREATE TABLE artist_music("
                              "music_id INT NOT NULL,"
                              "artist_id INT NOT NULL,"
                              "PRIMARY KEY (music_id, artist_id),"
                              "FOREIGN KEY (music_id) REFERENCES music(music_id) ON DELETE CASCADE,"
                              "FOREIGN KEY (artist_id) REFERENCES artist(artist_id) ON DELETE CASCADE"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建artist_music表失败");
        }

    }  catch (QString e) {
        return;
    }
}

SQLite::~SQLite()
{
    r = sqlite3_close(db);
    if (r != SQLITE_OK) logError("关闭保存数据库失败");
}

void SQLite::logError(QString error)
{
    if (this->error != nullptr) {
        error += " " + QString(this->error);
    }
    if (sqlite3_errmsg(db) != nullptr && QString(sqlite3_errmsg(db)) != QString(this->error)) {
        error += " " + QString(sqlite3_errmsg(db));
    }
    error = "SQLITE3 MODE：ERROR CODE " + QString::number(r) + ", " + error;
    qDebug()<<error;
    throw error;
}

int SQLite::countCallBack(void *data, int argc, char **argv, char **azColName)
{
    QString valueStr(*argv);
    int *value = static_cast<int *>(data);
    *value = valueStr.toInt();
    return SQLITE_OK;
}

bool SQLite::updateMusic(Music *music)
{
    QList<Music *> list;
    list.append(music);
    return updateMusic(list);
}

bool SQLite::updateMusic(QList<Music *> musicList)
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

bool SQLite::updateAlbum(Album *album)
{
    QList<Album *> list;
    list.append(album);
    return updateAlbum(list);
}

bool SQLite::updateAlbum(QList<Album *> albumList)
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

bool SQLite::updateArtist(Artist *artist)
{
    QList<Artist *> list;
    list.append(artist);
    return updateArtist(list);
}

bool SQLite::updateArtist(QList<Artist *> artistList)
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

bool SQLite::updateTable(Table *table)
{
    QList<Table *> list;
    list.append(table);
    return updateTable(list);
}

bool SQLite::updateTable(QList<Table *> tableList)
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

bool SQLite::updateTableMusic(QPair<int, int> pair)
{
    QList<QPair<int, int> > list;
    list.append(pair);
    return updateTableMusic(list);
}

bool SQLite::updateTableMusic(QList<QPair<int, int> > pairList)
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

bool SQLite::updateTableMusic(Table *table)
{
    QList<Table *> list;
    list.append(table);
    return updateTableMusic(list);
}

bool SQLite::updateTableMusic(QList<Table *> tableList)
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
            for (int j = 0; j < table->musics.size(); ++j) {
                r = sqlite3_reset(stmt); // 删除绑定
                if (r != SQLITE_OK) logError("重置 stmt 状态失败");
                r = sqlite3_bind_int(stmt, 1, table->tableId);
                if (r != SQLITE_OK) logError("绑定SQLITE "+QString(sql)+" 语句变量1失败");

                r = sqlite3_bind_int(stmt, 2, table->musics[j]->coreId);
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

bool SQLite::updateArtistMusic(QPair<int, int> pair)
{
    QList<QPair<int, int> > list;
    list.append(pair);
    return updateArtistMusic(list);
}

bool SQLite::updateArtistMusic(QList<QPair<int, int> > pairList)
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

bool SQLite::updateArtistMusic(Artist *artist)
{
    QList<Artist *> list;
    list.append(artist);
    return updateArtistMusic(list);
}

bool SQLite::updateArtistMusic(QList<Artist *> artistList)
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

                r = sqlite3_bind_int(stmt, 2, artist->musicList[j]->coreId);
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

bool SQLite::getArtistList(QList<Artist *> *artistList)
{
    try {
        const char *sql = "SELECT * FROM artist";
        r = sqlite3_exec(db, sql, [](void* data, int argc, char** argv, char** azColName)->int{
            qDebug()<<*argv;
            return SQLITE_OK;
        },artistList, &error);
        if (r != SQLITE_OK) logError("获取 artist 中的数据失败");
    } catch (QString e) {
        return false;
    }
    return true;
}


bool SQLite::selectNewMusic(QList<Music *> musicList, QList<Music *> newMusicList)
{
    sqlite3_stmt *checkUrlStmt = nullptr;
    sqlite3_stmt *checkMediaStmt = nullptr;

    try {
        const char *checkUrlSql = "SELECT count(*) FROM music WHERE url=?";
        const char *checkMediaSql = "SELECT count(*) FROM music "
                                    "JOIN artist_music ON music.music_id = artist_music.music_id "
                                    "JOIN artist ON artist_music.artist = artist.artist_id "
                                    "WHERE music.title=? AND artist.name=?";

        r = sqlite3_prepare_v2(db, checkUrlSql, -1, &checkUrlStmt, nullptr);
        if (r != SQLITE_OK) logError("检测 checkUrlSql 初始化失败");

        r = sqlite3_prepare_v2(db, checkMediaSql, -1, &checkMediaStmt, nullptr);
        if (r != SQLITE_OK) logError("检测 checkMediaSql 初始化失败");

        int value = 0;
        for (int i = 0; i < musicList.size(); ++i) {
            r = sqlite3_reset(checkUrlStmt);
            if (r != SQLITE_OK) logError("重置 checkUrlSql 失败");

            r = sqlite3_bind_text(checkUrlStmt, 1, musicList[i]->url.toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定 checkUrlSql 变量1失败");

            value = sqlite3_column_int(checkUrlStmt, 0);
            if (value > 0) break;

        }

    } catch (QString e) {
        sqlite3_finalize(checkUrlStmt);
        sqlite3_finalize(checkMediaStmt);
        return false;
    }
    sqlite3_finalize(checkUrlStmt);
    sqlite3_finalize(checkMediaStmt);
    return true;
}

bool SQLite::begin()
{
    try {
        r = sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("开始事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}

bool SQLite::rollback()
{
    try {
        r = sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("回滚事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}

bool SQLite::commit()
{
    try {
        r = sqlite3_exec(db, "COMMIT;", nullptr, nullptr, &error);
        if (r != SQLITE_OK) logError("结束事务失败");
    } catch (QString e) {
        return false;
    }
    return true;
}
