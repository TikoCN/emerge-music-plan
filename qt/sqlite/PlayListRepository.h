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

    [[nodiscard]] QString                 getAllList() const;
    [[nodiscard]] QList<int>              getPlayLists(int isDir) const;
    [[nodiscard]] PlayListPtr             getPlayList(int id) const;
    [[nodiscard]] PlayListPtr             getList(int id) const;
    [[nodiscard]] QHash<int, PlayListPtr> getPlayList(const QList<int> &idList) const;
    [[nodiscard]] QList<int>              getPlayListMusic(int id, int size, int start, int sort) const;
    [[nodiscard]] QList<int>              getPlayListMusicAll(int id, int sort) const;
    [[nodiscard]] QList<int>              getPlayListMusicInDir(int id) const;
    [[nodiscard]] QList<int>              getPlayListAllMusic(int id, int sort) const;
    [[nodiscard]] int                     getPlayListId(const QString &name) const;
    [[nodiscard]] QList<int>              getChildPlayList(int parentId) const;
    [[nodiscard]] QList<int>              getPlayingListMusic() const;
    [[nodiscard]] int                     checkPlayListName(const QString &name) const;

    [[nodiscard]] bool appendPlayList(const QString &playList) const;
    [[nodiscard]] bool appendPlayList(const QStringList &playList) const;
    [[nodiscard]] bool appendPlayListMusic(int id, const QList<int> &musicList) const;
    [[nodiscard]] bool appendPlayListMusic(const QPair<QString, QString> &pair) const;
    [[nodiscard]] bool appendPlayListMusic(const QList<QPair<QString, QString> > &pairList) const;
    [[nodiscard]] bool appendDirPlayList(const QString &url) const;
    [[nodiscard]] bool appendDirPlayList(const QStringList &urlList) const;
    [[nodiscard]] bool appendUserPlayList(const QString &name) const;
    [[nodiscard]] bool appendPlayingListMusic(int musicId, int position) const;
    [[nodiscard]] bool appendPlayingListMusic(const QList<int> &musicList, int start) const;

    bool updatePlayList(const PlayListPtr &playList) const;
    bool updatePlayListMusic(const QList<int> &musicIdList, int playlistNewId, int playlistOldId) const;
    bool updatePlayingListMusic(int musicId, int position) const;
    bool updatePlayingListMusic(const QList<int> &musicIdList, int start) const;

    bool movePlayListMusic(const QString &playListName, const QString &playListNameNew) const;
    bool addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const;

    [[nodiscard]] bool deletePlayingList(int position) const;
    bool               deletePlayList(const QString &playListName) const;
    bool               deletePlayListMusic(const QString &playListName, const QList<int> &musicIdList) const;
    bool               createPlayListDir(const QString &dirName) const;
    bool               deletePlayListDir(const QString &dirName) const;
};

#endif // PLAYLISTREPOSITORY_H
