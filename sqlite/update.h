#ifndef UPDATE_H
#define UPDATE_H

#include "core.h"
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
<<<<<<< Updated upstream:sqlite/update.h
#include "base/table.h"
=======
#include "base/playlist.h"
#include "append.h"
>>>>>>> Stashed changes:qt/sqlite/update.h

class Update : virtual public Core
{
public:
    // 更新条目
    bool updateMusic(MusicPtr music);
    bool updateMusic(QList<MusicPtr> musicList);

    bool updatePlayList(PlayListPtr playlist);

<<<<<<< Updated upstream:sqlite/update.h
    bool updateTableMusic(int musicId, int tableNewId, int tableOldId);
    bool updateTableMusic(QList<int> musicIdList, int tableNewId, int tableOldId);
=======
    bool updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId);
    Q_INVOKABLE bool updatePlayListMusic(QList<int> musicIdList, int playlistNewId, int playlistOldId);
>>>>>>> Stashed changes:qt/sqlite/update.h

    bool updateArtist(ArtistPtr artist);

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId);
    bool updateArtistMusic(QList<int> musicIdList, int artistNewId, int artistOldId);

    bool updateAlbum(AlbumPtr album);

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId);
    bool updateAlbumMusic(QList<int> musicIdList, int albumNewId, int albumOldId);
};

#endif // UPDATE_H
