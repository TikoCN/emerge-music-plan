#ifndef UPDATE_H
#define UPDATE_H

#include "core.h"
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
#include "base/table.h"

class Update : virtual public Core
{
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

};

#endif // UPDATE_H
