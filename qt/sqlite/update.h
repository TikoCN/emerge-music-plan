#ifndef UPDATE_H
#define UPDATE_H

#include "baseclass/music.h"
#include "baseclass/album.h"
#include "baseclass/artist.h"
#include "baseclass/playlist.h"
#include "append.h"

class Update : public Append
{
    Q_OBJECT
public:
    // 更新条目
    bool updateMusic(const MusicPtr &music);
    bool updateMusic(const QList<MusicPtr>& musicList);

    bool updatePlayList(const PlayListPtr& playlist);

    bool updatePlayListMusic(int musicId, int playlistNewId, int playlistOldId);
    Q_INVOKABLE bool updatePlayListMusic(const QList<int>& musicIdList, int playlistNewId, int playlistOldId);

    bool updateArtist(const ArtistPtr& artist);

    bool updateArtistMusic(int musicId, int artistNewId, int artistOldId);
    Q_INVOKABLE bool updateArtistMusic(const QList<int>& musicIdList, int artistNewId, int artistOldId);

    bool updateAlbum(const AlbumPtr& album);

    bool updateAlbumMusic(int musicId, int albumNewId, int albumOldId);
    Q_INVOKABLE bool updateAlbumMusic(const QList<int>& musicIdList, int albumNewId, int albumOldId);

    explicit Update(TLog * log) : Append(log) {};
    bool updateAlbumNameKey(const QStringList& albumName, const QStringList& albumNameKey);
    bool updateArtistNameKey(const QStringList& artistName, const QStringList& artistNameKey);
};

#endif // UPDATE_H
