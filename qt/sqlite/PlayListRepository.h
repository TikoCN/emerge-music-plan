#ifndef PLAYLISTREPOSITORY_H
#define PLAYLISTREPOSITORY_H

#include "Core.h"
#include "baseclass/Playlist.h"

class PlayListRepository {
private:
    Core *core;

public:
    explicit PlayListRepository(Core *core)
        : core(core) {
    }

    [[nodiscard]] QString     getAllList() const;
    [[nodiscard]] PlayListPtr get(int id) const;
    [[nodiscard]] QList<int>  getMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>  getMusicAll(int id, int sort) const;
    [[nodiscard]] int         allowName(const QString &name) const;
    [[nodiscard]] QList<int>  getPlayingListMusic() const;

    [[nodiscard]] bool appendDir(const QStringList &urlList) const;
    [[nodiscard]] bool appendUser(const QString &name) const;
    [[nodiscard]] bool appendPlayingMusic(const QList<int> &musicList, int start) const;

    bool update(const PlayListPtr &playList) const;
    bool updateMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    bool updatePlayingMusic(const QList<int> &musicIdList, int start) const;

    bool moveMusic(const QString &playListName, const QString &playListNameNew) const;
    bool addMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const;

    [[nodiscard]] bool deletePlayingList(int position) const;
};

#endif // PLAYLISTREPOSITORY_H
