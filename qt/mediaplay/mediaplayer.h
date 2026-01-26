#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMediaPlayer>
#include <QPixmap>
#include "mediaplay/lrcdatacontrol.h"

class MediaPlayer :public LrcDataControl
{
    Q_OBJECT
private:
    static MediaPlayer* instance;
    MediaPlayer(BaseTool *baseTool, DataActive *dataActive);

    int m_loopType;//播放种类

    QList<int> m_musicList;//正在播放列表
    int m_PlayingListId{};//正在播放歌曲的列表id
    int m_playingMusicId{};// 正在播放音乐id
    MusicPtr m_playingMusic;

    Q_PROPERTY(int playingMusicId READ playingMusicId CONSTANT)
    Q_PROPERTY(QList<int> musicList READ musicList CONSTANT)
    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

public:
    static MediaPlayer* getInstance(){
        return instance;
    }

    static void buildInstance(BaseTool *baseTool, DataActive *dataActive){
        if(instance == nullptr){
            instance = new MediaPlayer(baseTool, dataActive);
        }
    }

    static void freeInstance(){
            delete instance;
    }

    //播放下一首
    Q_INVOKABLE void playNext(int forward);
    //获得时间文本
    Q_INVOKABLE [[nodiscard]] QString getTimeString() const;
    [[nodiscard]] int getLoopType() const;
    void setLoopType(int newLoopType);
    void playMusicByListId(int musicListId);

    //播放音乐
    Q_INVOKABLE void buildPlayingListByMusicList(QList<int> list, int playMusicId = 0);
    Q_INVOKABLE void buildPlayingListByMusicId(int musicId);
    // 播放列表插入歌曲
    Q_INVOKABLE void insertPlayingListByMusicList(const QList<int>& list);
    Q_INVOKABLE void insertPlayingListByMusicId(int musicId);
    Q_INVOKABLE void appendPlayingListByMusicList(const QList<int>& list);
    Q_INVOKABLE void appendPlayingListByMusicId(int musicId);
    [[nodiscard]] int playingMusicId() const;
    [[nodiscard]] QList<int> musicList() const;

signals:

    //重建播放列表
    void musicListBuild();
    //将歌曲添加到播放播放列表
    void musicListAppend(int, int);

    void loopTypeChanged();
};

#endif // MEDIAPLAYER_H
