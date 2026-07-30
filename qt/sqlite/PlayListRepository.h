#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H

#include "Core.h"
#include "baseclass/Playlist.h"
#include "baseclass/MediaData.h"

class PlaylistRepository {
private:
    Core *core;

public:
    explicit PlaylistRepository(Core *core)
        : core(core) {
    }

    [[nodiscard]] PlaylistPtr get(int id) const;
    [[nodiscard]] QList<int>  getList(int start, int size, bool isDir) const;
    [[nodiscard]] QList<int>  getMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getMusicAll(int id, int sort) const;
    [[nodiscard]] int         allowName(const QString &name) const;
    [[nodiscard]] QList<int>  getPlayingListMusic() const;

    [[nodiscard]] bool appendDir(const QStringList &urlList) const;
    [[nodiscard]] bool appendUser(const QString &name) const;
    [[nodiscard]] bool appendMusic(const QList<MediaData> &dataList) const;
    [[nodiscard]] bool appendPlayingMusic(const QList<int> &musicList, int start) const;

    [[nodiscard]] bool update(const PlaylistPtr &playList) const;
    [[nodiscard]] bool updateMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    [[nodiscard]] bool updatePlayingMusic(const QList<int> &musicIdList, int start) const;

    [[nodiscard]] bool moveMusic(const QString &playListName, const QString &playListNameNew) const;
    [[nodiscard]] bool addMusicToPlaylist(const QString &sourcePlaylistName, const QString &targetPlaylistName) const;

    [[nodiscard]] bool deletePlayingList(int position) const;
};

#endif // PLAYLISTREPOSITORY_H
