#ifndef DATAACTIVE_H
#define DATAACTIVE_H
#include "DataManager.h"

class DataActive : public DataManager {
    Q_OBJECT

public:
    static DataActive &getInstance() {
        static DataActive instance;
        return instance;
    }

    QList<int> musicListSort(const QList<int> &, SORT_TYPE sort);

    void clearNullItem();

    void updateMusicLove(int musicId, bool isLove);
    void updateMusicLevel(int musicId, bool level);

    void updatePlaylistName(int playListId, const QString &name);
    void updatePlaylistSort(int playListId, int sort);

    void updateArtistName(int artistId, const QString &name);
    void updateArtistSort(int artistId, int sort);

    void updateAlbumName(int albumId, const QString &name);
    void updateAlbumSort(int albumId, int sort);

    /**
     * @brief 按配置来源下载歌曲封面。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downMusicCover(int musicId);
    /**
     * @brief 从 QQ 音乐下载歌曲封面。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downMusicCoverQQMusic(int musicId);
    /**
     * @brief 从网易云音乐下载歌曲封面。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downMusicCoverNetEase(int musicId);
    /**
     * @brief 从 Bing 下载封面。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downCoverBing(int musicId);
    /**
     * @brief 从百度下载封面。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downCoverBaidu(int musicId);
    /**
     * @brief 按配置来源下载歌词。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downLrc(int musicId);
    /**
     * @brief 从 QQ 音乐下载歌词。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downLrcFromQQMusic(int musicId);
    /**
     * @brief 从网易云音乐下载歌词。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downLrcFromNetEase(int musicId);
    /**
     * @brief 按配置来源下载歌手封面。
     * @param artistId 歌手 ID
     */
    Q_INVOKABLE void downArtistCover(int artistId);
    /**
     * @brief 按配置来源下载专辑封面。
     * @param albumId 专辑 ID
     */
    Q_INVOKABLE void downAlbumCover(int albumId);
    /**
     * @brief 从网易云音乐下载音乐元数据。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downMediaFromNetEase(int musicId);
    /**
     * @brief 从 QQ 音乐下载音乐元数据。
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void downMediaFromQQMusic(int musicId);

    void updateALLNameKey() const;

private:
    explicit DataActive();
    ~DataActive() override;
};

#endif // DATAACTIVE_H
