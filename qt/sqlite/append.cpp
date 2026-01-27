#include "append.h"
#include <QDir>
#include "namekey.h"
#include "baseclass/dataexception.h"

bool Append::appendMusic(const MediaData &data) {
    QList<MediaData> dataList;
    dataList.append(data);
    return appendMusic(dataList);
}

bool Append::appendMusic(const QList<MediaData> &dataList) {
    bool result = true;
    sqlite3_stmt *appendMusicStmt = nullptr;
    sqlite3_stmt *getAlbumIDStmt = nullptr;

    try {
        const auto getAlbumIDSql = "SELECT album_id FROM album WHERE name=? LIMIT 1";
        const auto appendMusicSql = "INSERT OR IGNORE INTO "
                "music(title, album_id, duration, insert_time, level, love, play_number, url) "
                "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
        stmtPrepare(&appendMusicStmt, appendMusicSql);
        stmtPrepare(&getAlbumIDStmt, getAlbumIDSql);

        for (const MediaData &data: dataList) {
            stmtReset(getAlbumIDStmt);
            stmtBindText(getAlbumIDStmt, 1, data.album);
            stmtStep(getAlbumIDStmt);

            const int album_id = sqlite3_column_int(getAlbumIDStmt, 0);
            stmtReset(appendMusicStmt);
            stmtBindText(appendMusicStmt, 1, data.title);
            stmtBindInt(appendMusicStmt, 2, album_id);
            stmtBindLong(appendMusicStmt, 3, data.duration);
            stmtBindLong(appendMusicStmt, 4, QDateTime::currentMSecsSinceEpoch());
            stmtBindInt(appendMusicStmt, 5, data.level);
            stmtBindInt(appendMusicStmt, 6, data.isLove);
            stmtBindInt(appendMusicStmt, 7, data.playNumber);
            stmtBindText(appendMusicStmt, 8, data.url);
            stmtStep(appendMusicStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendMusicStmt);
    stmtFree(getAlbumIDStmt);
    return result;
}

bool Append::appendAlbum(const QString &album) {
    QList<QString> dataList;
    dataList.append(album);
    return appendAlbum(dataList);
}

bool Append::appendAlbum(const QStringList &albumList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "INSERT OR IGNORE INTO album(name, key) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        NameKey key;

        for (const QString &i: albumList) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, i);
            stmtBindText(stmt, 2, key.find(i));
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendArtist(const QString &artist) {
    QList<QString> dataList;
    dataList.append(artist);
    return appendArtist(dataList);
}

bool Append::appendArtist(const QStringList &artistList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "INSERT OR IGNORE INTO artist(name, key) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        NameKey key;

        for (const QString &i: artistList) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, i);
            stmtBindText(stmt, 2, key.find(i));
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendDirPlayList(const QString &url) {
    QList<QString> dataList;
    dataList.append(url);
    return appendDirPlayList(dataList);
}

bool Append::appendDirPlayList(const QStringList &urlList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)";
        stmtPrepare(&stmt, sql);
        for (const QString &i: urlList) {
            stmtReset(stmt);
            QDir dir(i);

            stmtBindText(stmt, 1, dir.dirName());
            stmtBindInt(stmt, 2, 0);
            stmtBindText(stmt, 3, i);
            stmtBindInt(stmt, 4, 1);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendUserPlayList(const QString &name) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;

    try {
        const auto sql = "INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)";
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, name);
        stmtBindInt(stmt, 2, 0);
        stmtBindText(stmt, 3, "none");
        stmtBindInt(stmt, 4, 1);
        stmtStep(stmt);
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendArtistMusic(const QPair<QString, QString> &pair) {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendArtistMusic(dataList);
}

bool Append::appendArtistMusic(const QList<QPair<QString, QString> > &pairList) {
    bool result = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;

    try {
        const auto getIdSql = "SELECT"
                "(SELECT music_id FROM music WHERE url = ? LIMIT 1) AS music_id,"
                "(SELECT artist_id FROM artist WHERE name = ? LIMIT 1) AS artist_id";
        const auto appendSql = "INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)";
        stmtPrepare(&appendStmt, appendSql);
        stmtPrepare(&getIdStmt, getIdSql);

        for (const auto &[fst, snd]: pairList) {
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, fst);
            stmtBindText(getIdStmt, 2, snd);
            stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int artist_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, artist_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return result;
}

bool Append::appendArtistMusic(const int id, const QList<int> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        for (const int i: musicList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, i);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const int id, const QList<int> &musicList) {
    bool result = true;
    sqlite3_stmt *stmt = nullptr;
    try {
        const auto sql = "INSERT OR IGNORE INTO list_music(list_id, music_id) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        for (const int i: musicList) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, i);
            stmtStep(stmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }
    stmtFree(stmt);
    return result;
}

bool Append::appendPlayListMusic(const QPair<QString, QString> &pair) {
    QList<QPair<QString, QString> > dataList;
    dataList.append(pair);
    return appendPlayListMusic(dataList);
}

bool Append::appendPlayListMusic(const QList<QPair<QString, QString> > &pairList) {
    bool result = true;
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;

    try {
        const auto getIdSql = "SELECT"
                "(SELECT music_id FROM music WHERE url = ? LIMIT 1) AS music_id,"
                "(SELECT list_id FROM playlist WHERE url = ? LIMIT 1) AS list_id";
        const auto appendSql = "INSERT OR IGNORE INTO playlist_music(list_id, music_id) VALUES(?, ?)";
        stmtPrepare(&appendStmt, appendSql);
        stmtPrepare(&getIdStmt, getIdSql);

        for (const auto &[fst, snd]: pairList) {
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, fst);
            stmtBindText(getIdStmt, 2, snd);
            stmtStep(getIdStmt);

            const int music_id = sqlite3_column_int(getIdStmt, 0);
            const int list_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, list_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (const DataException &e) {
        tlog->logError(e.errorMessage());
        result = false;
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return result;
}
