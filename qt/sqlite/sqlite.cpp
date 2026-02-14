#include "sqlite.h"
#include <QDir>
#include "baseclass/dataexception.h"

SQLite *SQLite::getInstance() {
    return instance;
}

SQLite::SQLite(TLog *log)
    : Get(log) {
    m_error = nullptr;

    try {
        constexpr int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI | SQLITE_OPEN_FULLMUTEX;
        const QString &fileUrl = QDir::currentPath() + "/data/data.db";
        m_r = sqlite3_open_v2(fileUrl.toUtf8().constData(), &m_db, flags, nullptr);
        if (m_r != SQLITE_OK) throwError("打开数据库文件失败");
        int value = 0;

        // 启用外键约束
        const auto enableFK = "PRAGMA foreign_keys = ON;";
        m_r = sqlite3_exec(m_db, enableFK, nullptr, nullptr, nullptr);
        if (m_r != SQLITE_OK) throwError("启用外键失败");

        const auto check = "SELECT count(*) FROM sqlite_master WHERE type='table' AND ("
                "name='music' OR "
                "name='playlist' OR "
                "name='playlist_music' OR "
                "name='artist' OR "
                "name='artist_music' OR "
                "name='album' OR "
                "name='album_music' "
                ")";

        m_r = sqlite3_exec(m_db, check, countCallBack, &value, &m_error);
        if (m_r != SQLITE_OK) throwError("查询数据库中表完整性失败");

        if (value == 6) return;
        else if (value > 0) throwError("数据库异常，请删除 data.m_db 文件, 或者更新数据文件版本");

        tlog->logError(QString("只存在%1表").arg(value));
        // 检测 playlist
        {
            const auto sql = "CREATE TABLE playlist("
                    "list_id INTEGER PRIMARY KEY,"
                    "name TEXT NOT NULL,"
                    "url TEXT UNIQUE,"
                    "is_dir INT NOT NULL"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建 playlist 表失败");
        }

        // 检测 artist
        {
            const auto sql = "CREATE TABLE artist("
                    "artist_id INTEGER PRIMARY KEY,"
                    "name TEXT NOT NULL UNIQUE,"
                    "key TEXT NOT NULL"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建artist表失败");
        }

        // 检测 album
        {
            const auto sql = "CREATE TABLE album("
                    "album_id INTEGER PRIMARY KEY,"
                    "name TEXT NOT NULL UNIQUE,"
                    "key TEXT NOT NULL"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建album表失败");
        }

        // music
        {
            const auto sql = "CREATE TABLE music("
                    "music_id INTEGER PRIMARY KEY,"
                    "title TEXT NOT NULL,"
                    "album_id INT NOT NULL,"
                    "duration INT NOT NULL,"
                    "insert_time INT NOT NULL,"
                    "level INT NOT NULL,"
                    "love INT NOT NULL,"
                    "play_number INT NOT NULL,"
                    "url TEXT NOT NULL UNIQUE,"
                    "key TEXT NOT NULL "
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建music表失败");
        }

        // 检测 playlist_music
        {
            const auto sql = "CREATE TABLE playlist_music("
                    "music_id INTEGER NOT NULL,"
                    "list_id INTEGER NOT NULL,"
                    "PRIMARY KEY (music_id, list_id),"
                    "FOREIGN KEY (music_id) REFERENCES music(music_id) ON DELETE CASCADE,"
                    "FOREIGN KEY (list_id) REFERENCES playlist(list_id) ON DELETE CASCADE"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建playlist_music表失败");
        }

        // 检测 artist_music
        {
            const auto sql = "CREATE TABLE artist_music("
                    "music_id INTEGER NOT NULL,"
                    "artist_id INTEGER NOT NULL,"
                    "PRIMARY KEY (music_id, artist_id),"
                    "FOREIGN KEY (music_id) REFERENCES music(music_id) ON DELETE CASCADE,"
                    "FOREIGN KEY (artist_id) REFERENCES artist(artist_id) ON DELETE CASCADE"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建artist_music表失败");
        }

        // 检测 album_music
        {
            const auto sql = "CREATE TABLE album_music("
                    "music_id INTEGER NOT NULL,"
                    "album_id INTEGER NOT NULL,"
                    "PRIMARY KEY (music_id, album_id),"
                    "FOREIGN KEY (music_id) REFERENCES music(music_id) ON DELETE CASCADE,"
                    "FOREIGN KEY (album_id) REFERENCES album(album_id) ON DELETE CASCADE"
                    ")";
            // 执行sql
            m_r = sqlite3_exec(m_db, sql, nullptr, nullptr, &m_error);
            if (m_r != SQLITE_OK) throwError("创建album_music表失败");
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        return;
    }
}

SQLite::~SQLite() {
    m_r = sqlite3_close(m_db);
    if (m_r != SQLITE_OK) throwError("关闭保存数据库失败");
}

bool SQLite::selectNewMusic(const QFileInfoList &infoList, QFileInfoList *newInfoList) {
    sqlite3_stmt *stmt = nullptr;
    bool flag = true;
    try {
        const auto sql = "SELECT 1 FROM music WHERE url = ? LIMIT 1";
        stmtPrepare(&stmt, sql);
        for (const QFileInfo &i: infoList) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, i.filePath());
            stmtStep(stmt);
            if (sqlite3_column_int(stmt, 0) != 1)
                newInfoList->append(i);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        flag = false;
    }

    stmtFree(stmt);
    return flag;
}

QList<QString> SQLite::clearNullPlayListItem() {
    sqlite3_stmt *stmt = nullptr;
    QList<QString> removeList;
    try {
        QList<QString> urlList;

        // 判断删除文件
        const auto sql = "DELETE FROM playlist WHERE url = ?";
        stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            if (QString url = urlList.takeLast(); !QFile::exists(url)) {
                removeList.append(url);
                stmtReset(stmt);
                stmtBindText(stmt, 1, url);
                stmtStep(stmt);
            }
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return removeList;
}

QList<QString> SQLite::clearNullMusicItem() {
    sqlite3_stmt *stmt = nullptr;
    QList<QString> removeList;
    try {
        // 得到所有文件目录列表
        const auto urlSql = "SELECT url FROM playlist";
        QList<QString> urlList;
        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName) -> int {
            auto *list = static_cast<QList<QString> *>(data);
            list->append(QString(argv[0]));
            return SQLITE_OK;
        };
        sqlExec(urlSql, callback, &urlList);

        // 判断删除文件
        const auto sql = "DELETE FROM music WHERE url = ?";
        stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            if (QString url = urlList.takeLast(); !QFile::exists(url)) {
                removeList.append(url);
                stmtReset(stmt);
                stmtBindText(stmt, 1, url);
                stmtStep(stmt);
            }
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return removeList;
}
