#include "Sqlite.h"
#include <QDir>
#include "baseclass/DataException.h"

SQLite *SQLite::getInstance() {
    return instance;
}

SQLite::SQLite(TLog *log, BaseTool *tool)
    : Get(log, tool) {
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
                "name='playinglist' OR "
                "name='playlist_music' OR "
                "name='artist' OR "
                "name='artist_music' OR "
                "name='album' OR "
                "name='album_music' "
                ")";

        m_r = sqlite3_exec(m_db, check, countCallBack, &value, &m_error);
        if (m_r != SQLITE_OK) throwError("查询数据库中表完整性失败");

        if (value == 8) return;
        else if (value > 0) throwError("数据库异常，请删除 data.m_db 文件, 或者更新数据文件版本");

        m_loger->logError(QString("只存在%1表").arg(value));
        createTableMusic();
        createTablePlayList();
        createTablePlayinglist();
        createTablePlayListMuisc();
        createTableArtist();
        createTableArtistMusic();
        createTableAlbum();
        createTableAlbumMusic();
    } catch (const DataException &e) {
        m_loger->logError(e.errorMessage());
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
        m_loger->logError(e.errorMessage());
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
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return removeList;
}

void SQLite::createTableMusic() {
    const auto sql = QString("CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER PRIMARY KEY,"
                "%3 TEXT NOT NULL,"
                "%4 INT NOT NULL,"
                "%5 INT NOT NULL,"
                "%6 INT NOT NULL,"
                "%7 INT NOT NULL,"
                "%8 INT NOT NULL,"
                "%9 TEXT NOT NULL UNIQUE,"
                "%10 TEXT NOT NULL "
                ")")
            .arg(LiteralConstant::Table::MUSIC)
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Column::TITLE)
            .arg(LiteralConstant::Column::DURATION)
            .arg(LiteralConstant::Column::LAST_EDIT_TIME)
            .arg(LiteralConstant::Column::LEVEL)
            .arg(LiteralConstant::Column::IS_LOVE)
            .arg(LiteralConstant::Column::PLAY_NUMBER)
            .arg(LiteralConstant::Column::URL)
            .arg(LiteralConstant::Column::NAME_KEY);

    // 执行sql
    sqlExecute(sql.toUtf8(), "创建music表失败");
}

void SQLite::createTablePlayinglist() {
    // 检测 playinglist
    const auto sql = QString("CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER PRIMARY KEY,"
                "%3 INTEGER NOT NULL,"
                "FOREIGN KEY (%3) REFERENCES %4(%3) ON DELETE CASCADE"
                ")")
            .arg(LiteralConstant::Table::PLAYINGLIST)
            .arg(LiteralConstant::Column::POSITION)
            .arg(LiteralConstant::Column::MUSIC_ID)
            .arg(LiteralConstant::Table::MUSIC);
    // 执行sql
    sqlExecute(sql.toUtf8(), "创建 playinglist 表失败");
}

void SQLite::createTablePlayList() {
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER PRIMARY KEY,"
                "%3 TEXT NOT NULL,"
                "%4 TEXT UNIQUE,"
                "%5 INT NOT NULL, "
                "%6 INT NOT NULL "
                ")")
            .arg(LiteralConstant::Table::PLAYLIST) // %1
            .arg(LiteralConstant::Column::PLAYLIST_ID) // %2
            .arg(LiteralConstant::Column::PLAYLIST_NAME) // %3
            .arg(LiteralConstant::Column::URL) // %4
            .arg(LiteralConstant::Column::IS_DIR) // %5
            .arg(LiteralConstant::Column::SORT); // %6
    sqlExecute(sql.toUtf8(), "创建 playlist 表失败");
}

void SQLite::createTablePlayListMuisc() {
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER NOT NULL,"
                "%3 INTEGER NOT NULL,"
                "PRIMARY KEY (%2, %3),"
                "FOREIGN KEY (%2) REFERENCES %4(%2) ON DELETE CASCADE,"
                "FOREIGN KEY (%3) REFERENCES %5(%3) ON DELETE CASCADE"
                ")")
            .arg(LiteralConstant::Table::PLAYLIST_MUSIC) // %1
            .arg(LiteralConstant::Column::MUSIC_ID) // %2
            .arg(LiteralConstant::Column::PLAYLIST_ID) // %3
            .arg(LiteralConstant::Table::MUSIC) // %4
            .arg(LiteralConstant::Table::PLAYLIST); // %5
    sqlExecute(sql.toUtf8(), "创建 playlist_music 表失败");
}

void SQLite::createTableArtist() {
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER PRIMARY KEY,"
                "%3 TEXT NOT NULL UNIQUE,"
                "%4 TEXT NOT NULL, "
                "%5 INT NOT NULL"
                ")")
            .arg(LiteralConstant::Table::ARTIST) // %1
            .arg(LiteralConstant::Column::ARTIST_ID) // %2
            .arg(LiteralConstant::Column::ARTIST_NAME) // %3
            .arg(LiteralConstant::Column::NAME_KEY) // %4
            .arg(LiteralConstant::Column::SORT); // %5
    sqlExecute(sql.toUtf8(), "创建 artist 表失败");
}

void SQLite::createTableArtistMusic() {
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER NOT NULL,"
                "%3 INTEGER NOT NULL,"
                "PRIMARY KEY (%2, %3),"
                "FOREIGN KEY (%2) REFERENCES %4(%2) ON DELETE CASCADE,"
                "FOREIGN KEY (%3) REFERENCES %5(%3) ON DELETE CASCADE"
                ")")
            .arg(LiteralConstant::Table::ARTIST_MUSIC) // %1
            .arg(LiteralConstant::Column::MUSIC_ID) // %2
            .arg(LiteralConstant::Column::ARTIST_ID) // %3
            .arg(LiteralConstant::Table::MUSIC) // %4
            .arg(LiteralConstant::Table::ARTIST); // %5
    sqlExecute(sql.toUtf8(), "创建 artist_music 表失败");
}

void SQLite::createTableAlbum() {
    // 原 SQL 中 "sort INT NO NULL" 疑似笔误，已按 "NOT NULL" 处理
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER PRIMARY KEY,"
                "%3 TEXT NOT NULL UNIQUE,"
                "%4 TEXT NOT NULL, "
                "%5 INT NOT NULL"
                ")")
            .arg(LiteralConstant::Table::ALBUM) // %1
            .arg(LiteralConstant::Column::ALBUM_ID) // %2
            .arg(LiteralConstant::Column::ALBUM_NAME) // %3
            .arg(LiteralConstant::Column::NAME_KEY) // %4
            .arg(LiteralConstant::Column::SORT); // %5
    sqlExecute(sql.toUtf8(), "创建 album 表失败");
}

void SQLite::createTableAlbumMusic() {
    const auto sql = QString(
                "CREATE TABLE IF NOT EXISTS %1("
                "%2 INTEGER NOT NULL,"
                "%3 INTEGER NOT NULL,"
                "PRIMARY KEY (%2, %3),"
                "FOREIGN KEY (%2) REFERENCES %4(%2) ON DELETE CASCADE,"
                "FOREIGN KEY (%3) REFERENCES %5(%3) ON DELETE CASCADE"
                ")")
            .arg(LiteralConstant::Table::ALBUM_MUSIC) // %1
            .arg(LiteralConstant::Column::MUSIC_ID) // %2
            .arg(LiteralConstant::Column::ALBUM_ID) // %3
            .arg(LiteralConstant::Table::MUSIC) // %4
            .arg(LiteralConstant::Table::ALBUM); // %5
    sqlExecute(sql.toUtf8(), "创建 album_music 表失败");
}

QList<QString> SQLite::clearNullMusicItem() {
    sqlite3_stmt *stmt = nullptr;
    QList<QString> removeList;
    try {
        // 得到所有文件目录列表
        const auto urlSql = QString("SELECT %1 FROM %2")
                .arg(LiteralConstant::Column::URL)
                .arg(LiteralConstant::Table::PLAYINGLIST);
        QList<QString> urlList;
        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName) -> int {
            auto *list = static_cast<QList<QString> *>(data);
            list->append(QString(argv[0]));
            return SQLITE_OK;
        };
        sqlExecuteCallBack(urlSql.toUtf8(), callback, &urlList);

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
        m_loger->logError(e.errorMessage());
    }

    stmtFree(stmt);
    return removeList;
}