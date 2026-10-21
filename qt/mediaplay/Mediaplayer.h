#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QPixmap>
#include "mediaplay/LrcDataControl.h"
#include "sqlite/Sqlite.h"

class MediaPlayer : public LrcDataControl {
    Q_OBJECT

public:
    // 播放循环模式枚举，替代魔法数字 0/1/default
    enum class LoopType {
        Sequential = 0, // 顺序播放
        Random     = 1, // 随机播放
        RepeatOne  = 2  // 单曲循环
    };

private:
    MediaPlayer();

    QList<int> m_musicList;             //正在播放列表
    LoopType   m_loopType;              //播放种类
    int        m_playingMusicListId;    //正在播放歌曲的列表id（修正命名大写错误）
    int        m_playingMusicId;        // 正在播放音乐id
    MusicPtr   m_playingMusic;
    quint64    m_playbackRevision = 0;  // 播放请求版本，用于取消过期的自动切歌和启动操作。
    bool       m_startPending = false;  // 当前切歌请求是否仍在等待音源加载完成后启动。

    /**
     * @brief 检查 MP3 启动后的真实进度，仅对仍停在零位置的播放请求尝试一次恢复。
     */
    void schedulePlaybackStartCheck();

    Q_PROPERTY(int playingMusicId READ playingMusicId CONSTANT)
    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

public:
    static MediaPlayer &getInstance() {
        static MediaPlayer instance;
        return instance;
    }

    //播放下一首
    Q_INVOKABLE void playNext(int forward);
    //获得时间文本
    Q_INVOKABLE [[nodiscard]] QString getTimeString() const;
    [[nodiscard]] int                 getLoopType() const;
    void                              setLoopType(int newLoopType);
    Q_INVOKABLE void                  playMusicByListId(int musicListId);

    //播放音乐
    Q_INVOKABLE void buildPlayingList(QList<int> list, int playMusicId = 0);
    /**
     * @brief 从指定歌曲建立播放队列
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void buildPlayingListByMusicId(int musicId);
    Q_INVOKABLE void buildPlayingArtist(int artistId, int listId = 0);
    Q_INVOKABLE void buildPlayingAlbum(int albumId, int listId = 0);
    Q_INVOKABLE void buildPlayingPlaylist(int playListId, int listId = 0, int sort = -1);

    // 播放列表插入歌曲
    void             insertPlayingList(const QList<int> &list);
    /**
     * @brief 将指定歌曲插入到下一首
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void insertPlayingListByMusicId(int musicId);
    Q_INVOKABLE void insertPlayingArtist(int artistId);
    Q_INVOKABLE void insertPlayingAlbum(int albumId);
    Q_INVOKABLE void insertPlayingPlaylist(int playListId);

    void             appendPlayingList(const QList<int> &list);
    /**
     * @brief 将指定歌曲追加到播放队列
     * @param musicId 歌曲 ID
     */
    Q_INVOKABLE void appendPlayingListByMusicId(int musicId);
    Q_INVOKABLE void appendPlayingArtist(int artistId);
    Q_INVOKABLE void appendPlayingAlbum(int albumId);
    Q_INVOKABLE void appendPlayingPlaylist(int playListId);

    [[nodiscard]] int                    playingMusicId() const;
    Q_INVOKABLE [[nodiscard]] QList<int> getMusicList(int size, int start) const;

    Q_INVOKABLE void  initData();
    void              setPlayingMusicListId(int playingMusicListId);
    [[nodiscard]] int getPlayingMusicListId() const;
signals:
    //重建播放列表
    void musicListBuild();
    //将歌曲添加到播放播放列表
    void musicListAppend(int, int);

    void loopTypeChanged();
};

#endif // MEDIAPLAYER_H
