#include "Sqlite.h"
#include <QDir>
#include <QDebug>
#include "baseclass/DataException.h"
#include "../load/TaskCenter.h"

SQLite::SQLite()
    : getPort(&core),
      appendPort(&core),
      deletePort(&core),
      updatePort(&core) {
    core.m_error = nullptr;

    try {
        constexpr int  flags   = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI | SQLITE_OPEN_FULLMUTEX;
        const QString &fileUrl = QDir::currentPath() + "/data/data.db";
        core.m_r               = sqlite3_open_v2(fileUrl.toUtf8().constData(), &core.m_db, flags, nullptr);
        if (core.m_r != SQLITE_OK)
            core.throwError("打开数据库文件失败");
        int value = 0;

        const auto enableFK = "PRAGMA foreign_keys = ON;";
        core.m_r            = sqlite3_exec(core.m_db, enableFK, nullptr, nullptr, nullptr);
        if (core.m_r != SQLITE_OK)
            core.throwError("启用外键失败");

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

        core.m_r = sqlite3_exec(core.m_db, check, Core::countCallBack, &value, &core.m_error);
        if (core.m_r != SQLITE_OK)
            core.throwError("查询数据库中表完整性失败");

        if (value == 8)
            return;
        else if (value > 0)
            core.throwError("数据库异常，请删除 data.m_db 文件, 或者更新数据文件版本");

        TLog::getInstance().logError(QString("只存在%1表").arg(value));
        createTableMusic();
        createTablePlayList();
        createTablePlayinglist();
        createTablePlayListMusic();
        createTableArtist();
        createTableArtistMusic();
        createTableAlbum();
        createTableAlbumMusic();
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        return;
    }
}

SQLite::~SQLite() {
    if (core.m_db != nullptr) {
        core.m_r = sqlite3_close(core.m_db);
        if (core.m_r != SQLITE_OK) {
            const QString error = QString("关闭保存数据库失败: %1").arg(sqlite3_errmsg(core.m_db));
            qWarning() << error;
        }
        core.m_db = nullptr;
    }
}

bool SQLite::selectNewMusic(const QFileInfoList &infoList, QFileInfoList *newInfoList) {
    sqlite3_stmt *stmt = nullptr;
    bool          flag = true;
    try {
        const auto sql = "SELECT 1 FROM music WHERE url = ? LIMIT 1";
        core.stmtPrepare(&stmt, sql);
        for (const QFileInfo &i: infoList) {
            core.stmtReset(stmt);
            core.stmtBindText(stmt, 1, i.filePath());
            core.stmtStep(stmt);
            if (sqlite3_column_int(stmt, 0) != 1)
                newInfoList->append(i);
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
        flag = false;
    }

    Core::stmtFree(stmt);
    return flag;
}

QList<QString> SQLite::clearNullPlayListItem() {
    sqlite3_stmt * stmt = nullptr;
    QList<QString> removeList;
    try {
        QList<QString> urlList;

        // 判断删除文件
        const auto sql = "DELETE FROM playlist WHERE url = ?";
        core.stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            if (QString url = urlList.takeLast(); !QFile::exists(url)) {
                removeList.append(url);
                core.stmtReset(stmt);
                core.stmtBindText(stmt, 1, url);
                core.stmtStep(stmt);
            }
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }

    Core::stmtFree(stmt);
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
    core.sqlExecute(sql.toUtf8(), "创建music表失败");
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
    core.sqlExecute(sql.toUtf8(), "创建 playinglist 表失败");
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
                    .arg(LiteralConstant::Table::PLAYLIST)       // %1
                    .arg(LiteralConstant::Column::PLAYLIST_ID)   // %2
                    .arg(LiteralConstant::Column::PLAYLIST_NAME) // %3
                    .arg(LiteralConstant::Column::URL)           // %4
                    .arg(LiteralConstant::Column::IS_DIR)        // %5
                    .arg(LiteralConstant::Column::SORT);         // %6
    core.sqlExecute(sql.toUtf8(), "创建 playlist 表失败");
}

void SQLite::createTablePlayListMusic() {
    const auto sql = QString(
                         "CREATE TABLE IF NOT EXISTS %1("
                         "%2 INTEGER NOT NULL,"
                         "%3 INTEGER NOT NULL,"
                         "PRIMARY KEY (%2, %3),"
                         "FOREIGN KEY (%2) REFERENCES %4(%2) ON DELETE CASCADE,"
                         "FOREIGN KEY (%3) REFERENCES %5(%3) ON DELETE CASCADE"
                         ")")
                    .arg(LiteralConstant::Table::PLAYLIST_MUSIC) // %1
                    .arg(LiteralConstant::Column::MUSIC_ID)      // %2
                    .arg(LiteralConstant::Column::PLAYLIST_ID)   // %3
                    .arg(LiteralConstant::Table::MUSIC)          // %4
                    .arg(LiteralConstant::Table::PLAYLIST);      // %5
    core.sqlExecute(sql.toUtf8(), "创建 playlist_music 表失败");
}

void SQLite::createTableArtist() {
    const auto sql = QString(
                         "CREATE TABLE IF NOT EXISTS %1("
                         "%2 INTEGER PRIMARY KEY,"
                         "%3 TEXT NOT NULL UNIQUE,"
                         "%4 TEXT NOT NULL, "
                         "%5 INT NOT NULL"
                         ")")
                    .arg(LiteralConstant::Table::ARTIST)       // %1
                    .arg(LiteralConstant::Column::ARTIST_ID)   // %2
                    .arg(LiteralConstant::Column::ARTIST_NAME) // %3
                    .arg(LiteralConstant::Column::NAME_KEY)    // %4
                    .arg(LiteralConstant::Column::SORT);       // %5
    core.sqlExecute(sql.toUtf8(), "创建 artist 表失败");
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
                    .arg(LiteralConstant::Column::MUSIC_ID)    // %2
                    .arg(LiteralConstant::Column::ARTIST_ID)   // %3
                    .arg(LiteralConstant::Table::MUSIC)        // %4
                    .arg(LiteralConstant::Table::ARTIST);      // %5
    core.sqlExecute(sql.toUtf8(), "创建 artist_music 表失败");
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
                    .arg(LiteralConstant::Table::ALBUM)       // %1
                    .arg(LiteralConstant::Column::ALBUM_ID)   // %2
                    .arg(LiteralConstant::Column::ALBUM_NAME) // %3
                    .arg(LiteralConstant::Column::NAME_KEY)   // %4
                    .arg(LiteralConstant::Column::SORT);      // %5
    core.sqlExecute(sql.toUtf8(), "创建 album 表失败");
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
                    .arg(LiteralConstant::Column::MUSIC_ID)   // %2
                    .arg(LiteralConstant::Column::ALBUM_ID)   // %3
                    .arg(LiteralConstant::Table::MUSIC)       // %4
                    .arg(LiteralConstant::Table::ALBUM);      // %5
    core.sqlExecute(sql.toUtf8(), "创建 album_music 表失败");
}

QList<QString> SQLite::clearNullMusicItem() {
    sqlite3_stmt * stmt = nullptr;
    QList<QString> removeList;
    try {
        const auto urlSql = QString("SELECT %1 FROM %2")
                           .arg(LiteralConstant::Column::URL)
                           .arg(LiteralConstant::Table::PLAYINGLIST);
        QList<QString>         urlList;
        const sqlite3_callback callback = [](void *data, int argc, char **argv, char **azColName) -> int {
            auto *list = static_cast<QList<QString> *>(data);
            list->append(QString(argv[0]));
            return SQLITE_OK;
        };
        core.sqlExecuteCallBack(urlSql.toUtf8(), callback, &urlList);

        const auto sql = "DELETE FROM music WHERE url = ?";
        core.stmtPrepare(&stmt, sql);
        while (!urlList.isEmpty()) {
            if (QString url = urlList.takeLast(); !QFile::exists(url)) {
                removeList.append(url);
                core.stmtReset(stmt);
                core.stmtBindText(stmt, 1, url);
                core.stmtStep(stmt);
            }
        }
    } catch (const DataException &e) {
        TLog::getInstance().logError(e.errorMessage());
    }

    Core::stmtFree(stmt);
    return removeList;
}

QStringList SQLite::getArtistKeys() const {
    return getPort.getArtistKeys();
}

QList<int> SQLite::getArtistByKey(const QString &key, int size, int start) const {
    return getPort.getArtistByKey(key, size, start);
}

QList<int> SQLite::getArtistMusic(int id, int size, int start, int sort) const {
    return getPort.getArtistMusic(id, size, start, sort);
}

QList<int> SQLite::getArtistRandList() const {
    return getPort.getArtistRandList();
}

int SQLite::checkArtistName(const QString &name) const {
    return getPort.checkArtistName(name);
}

QStringList SQLite::getAlbumKeys() const {
    return getPort.getAlbumKeys();
}

QList<int> SQLite::getAlbumByKey(const QString &key, int size, int start) const {
    return getPort.getAlbumByKey(key, size, start);
}

QList<int> SQLite::getAlbumMusic(int id, int size, int start, int sort) const {
    return getPort.getAlbumMusic(id, size, start, sort);
}

QList<int> SQLite::getAlbumRandList() const {
    return getPort.getAlbumRandList();
}

int SQLite::checkAlbumName(const QString &name) const {
    return getPort.checkAlbumName(name);
}

QStringList SQLite::getMusicKeys() const {
    return getPort.getMusicKeys();
}

QList<int> SQLite::getMusicByKey(const QString &key, int size, int start) const {
    return getPort.getMusicByKey(key, size, start);
}

QList<int> SQLite::getMusicRandList(int length) const {
    return getPort.getMusicRandList(length);
}

QList<int> SQLite::getNewMusicList() const {
    return getPort.getNewMusicList();
}

QString SQLite::getAllList() const {
    return getPort.getAllList();
}

QList<int> SQLite::getReadMoreList() const {
    return getPort.getReadMoreList();
}

QList<int> SQLite::getPlayListMusic(int id, int size, int start, int sort) const {
    return getPort.getPlayListMusic(id, size, start, sort);
}

QList<int> SQLite::getPlayingListMusic() const {
    return getPort.getPlayingListMusic();
}

int SQLite::checkPlayListName(const QString &name) const {
    return getPort.checkPlayListName(name);
}

bool SQLite::updatePlayListMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const {
    return updatePort.updatePlayListMusic(musicIdList, playlistNewId, playlistOldId);
}

bool SQLite::updateArtistMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const {
    return updatePort.updateArtistMusic(musicIdList, artistNewId, artistOldId);
}

bool SQLite::updateAlbumMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const {
    return updatePort.updateAlbumMusic(musicIdList, albumNewId, albumOldId);
}

bool SQLite::moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const {
    return updatePort.moveAlbumMusic(albumName, albumNameNew);
}

bool SQLite::moveArtistMusic(const QString &artistName, const QString &artistNameNew) const {
    return updatePort.moveArtistMusic(artistName, artistNameNew);
}

bool SQLite::movePlayListMusic(const QString &playListName, const QString &playListNameNew) const {
    return updatePort.movePlayListMusic(playListName, playListNameNew);
}

bool SQLite::addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const {
    return appendPort.addArtistMusicToPlayList(artistName, playListName);
}

bool SQLite::addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const {
    return appendPort.addAlbumMusicToPlayList(albumName, playListName);
}

bool SQLite::addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const {
    return appendPort.addPlayListMusicToPlayList(sourcePlayListName, targetPlayListName);
}

void SQLite::startClearInvalidData() {
    QList<QPair<int, QString> > musicDataList = deletePort.getAllMusicData();
    emit startCheckFileExist(musicDataList);
}

void SQLite::onCheckFileFinished(const QList<int> &invalidMusicIds) {
    deletePort.clearInvalidData(invalidMusicIds);
}
