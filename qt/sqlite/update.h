#ifndef UPDATE_H
#define UPDATE_H

#include "core.h"
#include "base/music.h"
#include "base/album.h"
#include "base/artist.h"
#include "base/playlist.h"
#include "append.h"

class Update : public Append
{
    Q_OBJECT
public:
    // 更新条目
    bool updateMusic(MusicPtr music);
    bool updateMusic(QList<MusicPtr> musicList);

    bool updatePlayList(PlayListPtr playlist);

    bool updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId);
    Q_INVOKABLE bool updatePlayListMusic(QList<int> musicIdList, int playlistNewId, int playlistOldId);

    bool updateArtist(ArtistPtr artist);

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId);
    Q_INVOKABLE bool updateArtistMusic(QList<int> musicIdList, int artistNewId, int artistOldId);

    bool updateAlbum(AlbumPtr album);

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId);
    Q_INVOKABLE bool updateAlbumMusic(QList<int> musicIdList, int albumNewId, int albumOldId);
};

#endif // UPDATE_H
