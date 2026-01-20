#include "append.h"
#include <QDateTime>
#include <QDir>
#include "base.h"
#include "namekey.h"

bool Append::appendMusic(MediaData data)
{
    QList<MediaData> dataList;
    dataList.append(data);
    return appendMusic(dataList);
}

bool Append::appendMusic(QList<MediaData> dataList)
{
    sqlite3_stmt *appendMusicStmt = nullptr;
    sqlite3_stmt *getAlbumIDStmt = nullptr;
    const char *appendMusicSql = "INSERT OR IGNORE INTO "
                                 "music(title, album_id, duration, insert_time, level, love, play_number, url) "
                                 "VALUES(?, ?, ?, ?, ?, ?, ?, ?)";
    const char *getAlbumIDSql = "SELECT album_id FROM album WHERE name=? LIMIT 1";

    try {
        stmtPrepare(&appendMusicStmt, appendMusicSql);
        stmtPrepare(&getAlbumIDStmt, getAlbumIDSql);

        for (int i = 0; i < dataList.size(); ++i) {
            MediaData *data = &dataList[i];
            stmtReset(getAlbumIDStmt);
            stmtBindText(getAlbumIDStmt, 1, data->album);
            stmtStep(getAlbumIDStmt);

            int album_id = sqlite3_column_int(getAlbumIDStmt, 0);
            stmtReset(appendMusicStmt);
            stmtBindText(appendMusicStmt, 1, data->title);
            stmtBindInt(appendMusicStmt, 2, album_id);
            stmtBindInt(appendMusicStmt, 3, data->duration);
            stmtBindInt(appendMusicStmt, 4, QDateTime::currentMSecsSinceEpoch());
            stmtBindInt(appendMusicStmt, 5, data->level);
            stmtBindInt(appendMusicStmt, 6, data->isLove);
            stmtBindInt(appendMusicStmt, 7, data->playNumber);
            stmtBindText(appendMusicStmt, 8, data->url);
            stmtStep(appendMusicStmt);
        }
    } catch (QString e) {

    }

    stmtFree(appendMusicStmt);
    stmtFree(getAlbumIDStmt);
    return true;
}

bool Append::appendAlbum(QString album)
{
    QList<QString> dataList;
    dataList.append(album);
    return appendAlbum(dataList);
}

bool Append::appendAlbum(QStringList albumList)
{
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "INSERT OR IGNORE INTO album(name, key) VALUES(?, ?)";
    NameKey key;

    try {
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < albumList.size(); ++i) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, albumList[i]);
            stmtBindText(stmt, 2, key.find(albumList[i]));
            stmtStep(stmt);
        }
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Append::appendArtist(QString artist)
{
    QList<QString> dataList;
    dataList.append(artist);
    return appendArtist(dataList);
}

bool Append::appendArtist(QStringList artistList)
{
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "INSERT OR IGNORE INTO artist(name, key) VALUES(?, ?)";
    NameKey key;

    try {
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < artistList.size(); ++i) {
            stmtReset(stmt);
            stmtBindText(stmt, 1, artistList[i]);
            stmtBindText(stmt, 2, key.find(artistList[i]));
            stmtStep(stmt);
        }
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Append::appendDirPlayList(QString url)
{
    QList<QString> dataList;
    dataList.append(url);
    return appendDirPlayList(dataList);
}

bool Append::appendDirPlayList(QStringList urlList)
{
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)";

    try {
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < urlList.size(); ++i) {
            stmtReset(stmt);
            QDir dir(urlList[i]);

            stmtBindText(stmt, 1, dir.dirName());
            stmtBindInt(stmt, 2, 0);
            stmtBindText(stmt, 3, urlList[i]);
            stmtBindInt(stmt, 4, 1);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Append::appendUserPlayList(QString name)
{
    sqlite3_stmt *stmt = nullptr;
    const char *sql = "INSERT OR IGNORE INTO playlist(name, sort, url, is_dir) VALUES(?, ?, ?, ?)";

    try {
        stmtPrepare(&stmt, sql);
        stmtBindText(stmt, 1, name);
        stmtBindInt(stmt, 2, 0);
        stmtBindText(stmt, 3, "none");
        stmtBindInt(stmt, 4, 1);
        stmtStep(stmt);
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Append::appendArtistMusic(QPair<QString, QString> pair)
{
    QList<QPair<QString, QString>> dataList;
    dataList.append(pair);
    return appendArtistMusic(dataList);
}

bool Append::appendArtistMusic(QList<QPair<QString, QString> > pairList)
{
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;
    const char *appendSql = "INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)";
    const char *getIdSql = "SELECT"
                           "(SELECT music_id FROM music WHERE url = ? LIMIT 1) AS music_id,"
                           "(SELECT artist_id FROM artist WHERE name = ? LIMIT 1) AS artist_id";

    try {
        stmtPrepare(&appendStmt, appendSql);
        stmtPrepare(&getIdStmt, getIdSql);

        for (int i = 0; i < pairList.size(); ++i) {
            QPair<QString, QString> *pair = &pairList[i];
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, pair->first);
            stmtBindText(getIdStmt, 2, pair->second);
            stmtStep(getIdStmt);

            int music_id = sqlite3_column_int(getIdStmt, 0);
            int artist_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, artist_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (QString e) {
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return true;
}

bool Append::appendArtistMusic(int id, QList<int> musicList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR IGNORE INTO artist_music(artist_id, music_id) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, musicList[i]);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }

    stmtFree(stmt);
    return true;
}

bool Append::appendPlayListMusic(int id, QList<int> musicList)
{
    sqlite3_stmt *stmt = nullptr;
    try {
        const char *sql = "INSERT OR IGNORE INTO list_music(list_id, music_id) VALUES(?, ?)";
        stmtPrepare(&stmt, sql);
        for (int i = 0; i < musicList.size(); ++i) {
            stmtReset(stmt);
            stmtBindInt(stmt, 1, id);
            stmtBindInt(stmt, 2, musicList[i]);
            stmtStep(stmt);
        }
    } catch (QString e) {
    }
    stmtFree(stmt);
    return true;
}

bool Append::appendPlayListMusic(QPair<QString, QString> pair)
{
    QList<QPair<QString, QString>> dataList;
    dataList.append(pair);
    return appendPlayListMusic(dataList);
}

bool Append::appendPlayListMusic(QList<QPair<QString, QString> > pairList)
{
    sqlite3_stmt *appendStmt = nullptr;
    sqlite3_stmt *getIdStmt = nullptr;
    const char *appendSql = "INSERT OR IGNORE INTO playlist_music(list_id, music_id) VALUES(?, ?)";
    const char *getIdSql = "SELECT"
                           "(SELECT music_id FROM music WHERE url = ? LIMIT 1) AS music_id,"
                           "(SELECT list_id FROM playlist WHERE url = ? LIMIT 1) AS list_id";

    try {
        stmtPrepare(&appendStmt, appendSql);
        stmtPrepare(&getIdStmt, getIdSql);

        for (int i = 0; i < pairList.size(); ++i) {
            QPair<QString, QString> *pair = &pairList[i];
            stmtReset(getIdStmt);
            stmtBindText(getIdStmt, 1, pair->first);
            stmtBindText(getIdStmt, 2, pair->second);
            stmtStep(getIdStmt);

            int music_id = sqlite3_column_int(getIdStmt, 0);
            int list_id = sqlite3_column_int(getIdStmt, 1);
            stmtReset(appendStmt);
            stmtBindInt(appendStmt, 1, list_id);
            stmtBindInt(appendStmt, 2, music_id);
            stmtStep(appendStmt);
        }
    } catch (QString e) {
    }

    stmtFree(appendStmt);
    stmtFree(getIdStmt);
    return true;
}
