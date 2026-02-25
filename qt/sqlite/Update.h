#ifndef UPDATE_H
#define UPDATE_H

#include "baseclass/Music.h"
#include "baseclass/Album.h"
#include "baseclass/Artist.h"
#include "baseclass/Playlist.h"
#include "Append.h"

class Update : public Append
{
    Q_OBJECT
public:

    explicit Update(TLog * log) : Append(log) {};

    // 更新条目
    bool updateMusic(const MusicPtr &music);
    bool updateMusic(const QList<MusicPtr>& musicList);

    bool updatePlayList(const PlayListPtr& playlist);

    bool updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId);
    Q_INVOKABLE bool updatePlayListMusic(const QList<int>& musicIdList, int playlistNewId, int playlistOldId);
    bool updatePlayingListMusic(int musicId, int position);
    bool updatePlayingListMusic(const QList<int>& musicIdList, int start);

    bool updateArtist(const ArtistPtr& artist);

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId);
    Q_INVOKABLE bool updateArtistMusic(const QList<int>& musicIdList, int artistNewId, int artistOldId);

    bool updateAlbum(const AlbumPtr& album);

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId);
    Q_INVOKABLE bool updateAlbumMusic(const QList<int>& musicIdList, int albumNewId, int albumOldId);

    bool updateAlbumNameKey(const QStringList& albumName, const QStringList& albumNameKey);
    bool updateArtistNameKey(const QStringList& artistName, const QStringList& artistNameKey);
};

#endif // UPDATE_H
