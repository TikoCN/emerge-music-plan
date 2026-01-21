#include "sqlite.h"
#include <QDebug>
#include <QDir>

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

        const char *check = "SELECT count(*) FROM sqlite_master WHERE type='playlist' AND ("
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
    sqlite3_stmt *stmt = nullptr;
    bool flag = true;
    try {
        const char* sql = "SELECT 1 FROM music WHERE url = ? LIMIT 1";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < infoList.size(); ++i) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, infoList[i].filePath());
            stmtStep(stmt);
            if (sqlite3_column_int(stmt, 0) != 1)
                newInfoList->append(infoList[i]);
        }
    } catch (QString e) {
        TLog::getInstance()->logError(e);
        flag = false;
    }

    stmtFree(stmt);
    return flag;
}

QList<QString> SQLite::clearNullPlayListItem()
{
    sqlite3_stmt *stmt = nullptr;
    QList<QString> removeList;
    try {
        QList<QString> urlList ;

        // 判断删除文件
        const char* sql = "DELETE FROM playlist WHERE url = ?";
        stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            QString url = urlList.takeLast();
            if (!QFile::exists(url)){
                removeList.append(url);
                stmtReset(stmt);
                stmtBindText(stmt, 1, url);
                stmtStep(stmt);
            }
        }
    } catch (QString e) {
        TLog::getInstance()->logError(e);
    }

    stmtFree(stmt);
    return removeList;
}

QList<QString> SQLite::clearNullMusicItem()
{
    sqlite3_stmt *stmt = nullptr;
    QList<QString> removeList;
    try {
        // 得到所有文件目录列表
        const char* urlSql = "SELECT url FROM playlist";
        QList<QString> urlList ;
        sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName) -> int{
            QList<QString> *list = static_cast<QList<QString> *>(data);
            list->append(QString(argv[0]));
            return SQLITE_OK;
        };
        sqlExec(urlSql, callback, &urlList);

        // 判断删除文件
        const char* sql = "DELETE FROM music WHERE url = ?";
        stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            QString url = urlList.takeLast();
            if (!QFile::exists(url)){
                removeList.append(url);
                stmtReset(stmt);
                stmtBindText(stmt, 1, url);
                stmtStep(stmt);
            }
        }
    } catch (QString e) {
        TLog::getInstance()->logError(e);
    }

    stmtFree(stmt);
    return removeList;
}

