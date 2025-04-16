#ifndef SQLITE_H
#define SQLITE_H
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
#include "base/table.h"
#include <sqlite3.h>
#include <QObject>

class SQLite : public QObject
{
    Q_OBJECT
public:
    static SQLite *instance;
    static SQLite *getInstance();

    static void buildInstance(){
        if (instance == nullptr) {
            instance = new SQLite;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

private:
    sqlite3 *db;
    int r;
    char *error;
    SQLite();
    ~SQLite();
    void logError(QString error);

    static int countCallBack(void *data, int argc, char **argv, char **azColName);

public:
    // 更新条目
    bool updateMusic(Music *music);
    bool updateMusic(QList<Music *> musicList);

    bool updateAlbum(Album *album);
    bool updateAlbum(QList<Album *> albumList);

    bool updateArtist(Artist *artist);
    bool updateArtist(QList<Artist *> artistList);

    bool updateTable(Table *table);
    bool updateTable(QList<Table *> tableList);

    bool updateTableMusic(QPair<int ,int> pair);
    bool updateTableMusic(QList<QPair<int ,int>> pairList);
    bool updateTableMusic(Table *table);
    bool updateTableMusic(QList<Table *> tableList);

    bool updateArtistMusic(QPair<int ,int> pair);
    bool updateArtistMusic(QList<QPair<int ,int>> pairList);
    bool updateArtistMusic(Artist *artist);
    bool updateArtistMusic(QList<Artist *> artistList);

    // 获得数据
    bool getArtistList(QList<Artist *> *artistList);
    bool getAlumbList(QList<Album *> *albumList);
    bool getMusicList(QList<Music *> *musicList);
    bool getTableList(QList<Table *> *tableList);

    // 获得新的条目
    bool selectNewMusic(QList<Music *> musicList, QList<Music *> newMusicList);

    bool commit();

    bool begin();

    bool rollback();
};

#endif // SQLITE_H
