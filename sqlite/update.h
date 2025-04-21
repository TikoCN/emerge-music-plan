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

    bool updateTable(Table *table);

    bool updateTableMusic(int musicId, int tableNewId, int tableOldId);
    bool updateTableMusic(QList<int> musicIdList, int tableNewId, int tableOldId);

    bool updateArtist(Artist *artist);

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId);
    bool updateArtistMusic(QList<int> musicIdList, int artistNewId, int artistOldId);

    bool updateAlbum(Album *album);

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId);
    bool updateAlbumMusic(QList<int> musicIdList, int albumNewId, int albumOldId);
};

#endif // UPDATE_H
