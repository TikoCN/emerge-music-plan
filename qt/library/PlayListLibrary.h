#ifndef PLAYLISTLIBRARY_H
#define PLAYLISTLIBRARY_H

#include <QObject>
#include "baseclass/Playlist.h"
#include "datacore/DataLoader.h"

class PlaylistModel;

class PlaylistLibrary : public QObject {
    Q_OBJECT

public:
    static PlaylistLibrary &getInstance() {
        static PlaylistLibrary instance;
        return instance;
    }

    Q_INVOKABLE static void appendUser(const QString &name);

    Q_INVOKABLE static void updateName(int playListId, const QString &name);
    Q_INVOKABLE static void updateSort(int playListId, int sort);

    Q_INVOKABLE static Playlist getData(int id);

    Q_INVOKABLE static int allowName(const QString &name);

    Q_INVOKABLE static bool moveMusic(const QString &playListName, const QString &playListNameNew);
    Q_INVOKABLE static bool addMusicToPlaylist(const QString &sourcePlaylistName, const QString &targetPlaylistName);
    /**
     * @brief 将单首歌曲添加到用户播放列表
     * @param musicId 歌曲 ID
     * @param playlistId 播放列表 ID
     * @return 操作是否成功
     */
    Q_INVOKABLE static bool addSingleMusicToPlaylist(int musicId, int playlistId);
    /**
     * @brief 从播放列表移除单首歌曲
     * @param musicId 歌曲 ID
     * @param playlistId 播放列表 ID
     * @return 操作是否成功
     */
    Q_INVOKABLE static bool removeMusicFromPlaylist(int musicId, int playlistId);
    /**
     * @brief 将单首歌曲移动到另一播放列表
     * @param musicId 歌曲 ID
     * @param sourcePlaylistId 原列表 ID
     * @param targetPlaylistId 目标列表 ID
     * @return 操作是否成功
     */
    Q_INVOKABLE static bool moveMusicToPlaylist(int musicId, int sourcePlaylistId, int targetPlaylistId);

private:
    explicit PlaylistLibrary()  = default;
    ~PlaylistLibrary() override = default;
};

#endif // PLAYLISTLIBRARY_H
