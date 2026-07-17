#ifndef UPDATE_H
#define UPDATE_H

#include "baseclass/Music.h"
#include "baseclass/Album.h"
#include "baseclass/Artist.h"
#include "baseclass/Playlist.h"
#include "Core.h"

class Update {
private:
    Core *core;

public:
    explicit Update(Core *core)
        : core(core) {
    };

    // 更新条目
    bool updateMusic(const MusicPtr &music) const;
    bool updateMusic(const QList<MusicPtr> &musicList) const;

    bool updatePlayList(const PlayListPtr &playlist) const;

    bool updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId) const;
    bool updatePlayListMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    bool updatePlayingListMusic(int musicId, int position) const;
    bool updatePlayingListMusic(const QList<int> &musicIdList, int start) const;

    bool updateArtist(const ArtistPtr &artist) const;

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId) const;
    bool updateArtistMusic(const QList<int> &musicIdList, int artistNewId, int artistOldId) const;

    bool updateAlbum(const AlbumPtr &album) const;

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId) const;
    bool updateAlbumMusic(const QList<int> &musicIdList, int albumNewId, int albumOldId) const;

    bool updateAlbumNameKey(const QStringList &albumName, const QStringList &albumNameKey) const;
    bool updateArtistNameKey(const QStringList &artistName, const QStringList &artistNameKey) const;
};

#endif // UPDATE_H
