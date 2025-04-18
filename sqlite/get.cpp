#include "get.h"

QStringList Get::getArtistKeyList()
{
    QStringList keyList;
    try {
        const char *sql = "SELECT name FROM artist";

        r = sqlite3_exec(db, sql, [](void *data, int argc, char **argv, char **azColName)->int{
            QStringList *keyList = static_cast<QStringList *>(data);
            keyList->append(QString(*argv));
            return SQLITE_OK;
        }, &keyList, &error);

        if (r != SQLITE_OK) logError("得到 artist 所有 name 失败");
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

QHash<int, Artist *> Get::getArtist(QString key)
{
    QHash<int, Artist *> artistHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, artist_id, key FROM artist WHERE key=?";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        while (!stmtStep(stmt)) {
            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            Artist *artist = new Artist(name, id, key);
            artistHash.insert(id, artist);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return artistHash;
    }
    sqlite3_finalize(stmt);
    return artistHash;
}

Artist *Get::getArtist(int id)
{
    Artist *artist;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, artist_id, key FROM artist WHERE artist_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
        int id = sqlite3_column_int(stmt, 1);
        QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
        Artist *artist = new Artist(name, id, key);
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return artist;
    }
    sqlite3_finalize(stmt);
    return artist;
}

QHash<int, Artist *> Get::getArtist(QList<int> idList)
{
    QHash<int, Artist *> artistHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, artist_id, key FROM artist WHERE artist_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < idList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, idList[i]);
            stmtStep(stmt);

            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            Artist *artist = new Artist(name, id, key);
            artistHash.insert(id, artist);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return artistHash;
    }
    sqlite3_finalize(stmt);
    return artistHash;
}

QStringList Get::getAlbumKeyList()
{
    QStringList keyList;
    try {
        const char *sql = "SELECT name FROM album";

        r = sqlite3_exec(db, sql, [](void *data, int argc, char **argv, char **azColName)->int{
            QStringList *keyList = static_cast<QStringList *>(data);
            keyList->append(QString(*argv));
            return SQLITE_OK;
        }, &keyList, &error);

        if (r != SQLITE_OK) logError("得到 album 所有 name 失败");
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

QHash<int ,Album *> Get::getAlbum(QString key)
{
    QHash<int ,Album *> albumHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, album_id, key FROM album WHERE key=?";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, key);
        while (!stmtStep(stmt)) {
            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            Album *album = new Album(name, id, key);
            albumHash.insert(id, album);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return albumHash;
    }
    sqlite3_finalize(stmt);
    return albumHash;
}

Album *Get::getAlbum(int id)
{
    Album *album;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, album_id, key FROM album WHERE album_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
        int id = sqlite3_column_int(stmt, 1);
        QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
        Album *album = new Album(name, id, key);
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return album;
    }
    sqlite3_finalize(stmt);
    return album;
}

QHash<int ,Album *> Get::getAlbum(QList<int> idList)
{
    QHash<int ,Album *> albumHash;
    sqlite3_stmt *stmt = nullptr;

    try {
        const char *sql = "SELECT name, Album_id, key FROM Album WHERE album_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < idList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, idList[i]);
            stmtStep(stmt);

            QString name = QString::fromUtf8(sqlite3_column_text(stmt, 0));
            int id = sqlite3_column_int(stmt, 1);
            QString key = QString::fromUtf8(sqlite3_column_text(stmt, 2));
            Album *album = new Album(name, id, key);
            albumHash.insert(id, album);
        }
    } catch (QString e) {
        sqlite3_finalize(stmt);
        return albumHash;
    }
    sqlite3_finalize(stmt);
    return albumHash;
}

QString Get::getMusicUrl(int id)
{
    sqlite3_stmt *stmt = nullptr;
    QString url;
    try {
        const char *sql = "SELECT url FROM music WHERE music_id=? LIMIT 1";
        stmtPrepare(&stmt, sql);
        stmtBindInt(stmt, 1, id);
        stmtStep(stmt);
        url = QString::fromUtf8(sqlite3_column_text(stmt, 0));
    } catch (QString e) {
    }
    sqlite3_finalize(stmt);
    return url;
}

// Music *Get::getMusic(int id)
// {

// }

// QList<Music *> Get::getMusic(QList<int> idList)
// {

// }

QHash<int, QString> Get::getAllList()
{
    QHash<int, QString> keyList;
    try {
        const char *sql = "SELECT name, list_id FROM playlist";

        r = sqlite3_exec(db, sql, [](void *data, int argc, char **argv, char **azColName)->int{
            QHash<int, QString> *keyList = static_cast<QHash<int, QString> *>(data);
            keyList->insert(QString(argv[1]).toInt(), QString(argv[0]));
            return SQLITE_OK;
        }, &keyList, &error);

        if (r != SQLITE_OK) logError("得到 playlist 所有 name, list_id 失败");
    } catch (QString e) {
        return keyList;
    }
    return keyList;
}

// QList<Table *> Get::getList(int id)
// {

// }
