#include "sqlite.h"
#include "ffmpeg.h"
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
                              "list_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL,"
                              "sort INT NOT NULL,"
                              "url TEXT UNIQUE,"
                              "is_dir INT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建 playlist 表失败");
        }

        // 检测 artist
        {
            const char *sql = "CREATE TABLE artist("
                              "artist_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL UNIQUE,"
                              "key TEXT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建artist表失败");
        }

        // 检测 album
        {
            const char *sql = "CREATE TABLE album("
                              "album_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "name TEXT NOT NULL UNIQUE,"
                              "key TEXT NOT NULL"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建album表失败");
        }

        // music
        {
            const char *sql = "CREATE TABLE music("
                              "music_id INTEGER PRIMARY KEY AUTOINCREMENT,"
                              "title TEXT NOT NULL,"
                              "album_id INT NOT NULL,"
                              "duration INT NOT NULL,"
                              "insert_time INT NOT NULL,"
                              "level INT NOT NULL,"
                              "love INT NOT NULL,"
                              "play_number INT NOT NULL,"
                              "url TEXT NOT NULL UNIQUE,"
                              "FOREIGN KEY (album_id) REFERENCES album(album_id) ON DELETE CASCADE"
                              ")";
            // 执行sql
            r = sqlite3_exec(db, sql, NULL, NULL, &error);
            if (r != SQLITE_OK) logError("创建music表失败");
        }

        // 检测 playlist_music
        {
            const char *sql = "CREATE TABLE playlist_music("
                              "music_id INTEGER NOT NULL,"
                              "list_id INTEGER NOT NULL,"
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
                              "music_id INTEGER NOT NULL,"
                              "artist_id INTEGER NOT NULL,"
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

bool SQLite::selectNewMusic(QFileInfoList infoList, QFileInfoList *newInfoList)
{
    sqlite3_stmt *checkUrlStmt = nullptr;
    sqlite3_stmt *checkMediaStmt = nullptr;

    try {
        const char *checkUrlSql = "SELECT count(*) FROM music WHERE url=?";
        const char *checkMediaSql = "SELECT count(*) FROM music "
                                    "JOIN album ON album.album_id = music.album_id "
                                    "JOIN artist_music ON music.music_id = artist_music.music_id "
                                    "JOIN artist ON artist_music.artist_id = artist.artist_id "
                                    "WHERE music.title=? AND artist.name=? AND album.name=?";

        r = sqlite3_prepare_v2(db, checkUrlSql, -1, &checkUrlStmt, nullptr);
        if (r != SQLITE_OK) logError("检测 checkUrlSql 初始化失败");

        r = sqlite3_prepare_v2(db, checkMediaSql, -1, &checkMediaStmt, nullptr);
        if (r != SQLITE_OK) logError("检测 checkMediaSql 初始化失败");

        int value = 0;
        for (int i = 0; i < infoList.size(); ++i) {
            r = sqlite3_reset(checkUrlStmt);
            if (r != SQLITE_OK) logError("重置 checkUrlSql 失败");

            r = sqlite3_bind_text(checkUrlStmt, 1, infoList[i].filePath().toStdString().c_str(), -1, SQLITE_TRANSIENT);
            if (r != SQLITE_OK) logError("绑定 checkUrlSql 变量1失败");

            value = sqlite3_column_int(checkUrlStmt, 0);
            if (value > 0) break;

            MediaData data;
            FFmpeg ff;
            ff.getDict(&data, infoList[i].filePath());
            for (int j = 0; j < data.artistList.size(); ++j) {
                r = sqlite3_reset(checkMediaStmt);
                if (r != SQLITE_OK) logError("重置 checkMediaSql 失败");

                r = sqlite3_bind_text(checkMediaStmt, 1, data.title.toStdString().c_str(), -1, SQLITE_TRANSIENT);
                if (r != SQLITE_OK) logError("绑定 checkMediaSql 变量1失败");

                r = sqlite3_bind_text(checkMediaStmt, 2, data.artistList[j].toStdString().c_str(), -1, SQLITE_TRANSIENT);
                if (r != SQLITE_OK) logError("绑定 checkMediaSql 变量2失败");

                r = sqlite3_bind_text(checkMediaStmt, 3, data.album.toStdString().c_str(), -1, SQLITE_TRANSIENT);
                if (r != SQLITE_OK) logError("绑定 checkMediaSql 变量3失败");

                value = sqlite3_column_int(checkMediaStmt, 0);
                if (value > 0) break;
            }
            if (value <= 0) newInfoList->append(infoList[i]);
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

