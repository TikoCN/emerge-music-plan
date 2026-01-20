#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "musiccore.h"
#include "base/lrcdata.h"
#include <QMediaPlayer>
#include <QAudioBufferOutput>
#include <QAudioOutput>
#include <QPixmap>

class MediaPlayer :public QObject
{
    Q_OBJECT
private:
    static MediaPlayer* instance;
    MediaPlayer();

    QMediaPlayer *m_player;//播放设备
    QAudioOutput *m_audioOutput;//音频输出
    QAudioBufferOutput *m_bufferOutput;//缓冲区输出

    QList<int> m_musicList;//正在播放列表
    QVector<double> m_allSamples;//处理之后的音乐样本
    QList<LrcData *> m_lrcList;//歌词

    MusicCore *m_core;//音乐数据中心
    int m_loopType;//播放种类
    int m_PlayingListId;//正在播放歌曲的列表id
    int m_playingMusicId;// 正在播放音乐id
    MusicPtr m_playingMusic;
    LrcData *m_playingLrc;// 正在播放歌词行

    Q_PROPERTY(QMediaPlayer *player READ getPlayer CONSTANT)

    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

    Q_PROPERTY(QVector<double> allSamples READ getAllSamples CONSTANT)

    Q_PROPERTY(QAudioOutput *audioOutput READ getAudioOutput CONSTANT)

    Q_PROPERTY(QList<LrcData *> lrcList READ getLrcList CONSTANT)

    Q_PROPERTY(LrcData *playingLrc READ getPlayingLrc CONSTANT)

    Q_PROPERTY(int playingMusicId READ playingMusicId CONSTANT)

    Q_PROPERTY(QList<int> musicList READ musicList CONSTANT)

public:
    static MediaPlayer* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new MediaPlayer;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    //删除以及加载的数据
    Q_INVOKABLE void clearData();
    void clearMusicList();

    //更新播放设备
    void updateAudioOutPut();

    //播放音乐
    Q_INVOKABLE void buildPlayingListByMusicList(QList<int> list, int playMusicId = 0);
    Q_INVOKABLE void buildPlayingListByMusicId(int musicId);

    // 播放列表插入歌曲
    Q_INVOKABLE void insertPlayingListByMusicList(QList<int> list);
    Q_INVOKABLE void insertPlayingListByMusicId(int musicId);
    Q_INVOKABLE void appendPlayingListByMusicList(QList<int> list);
    Q_INVOKABLE void appendPlayingListByMusicId(int musicId);

    //加载歌词
    void loadLrcList();

    Q_INVOKABLE void playMusicByListId(int musicListId);

    //播放下一首
    Q_INVOKABLE void playNext(int forward);

    //获得时间文本
    Q_INVOKABLE QString getTimeString();

    //选择当前播放歌词
    void selectPlayLrc(qint64 time);

    //跳转到目标歌词
    Q_INVOKABLE void turnToLrc(int lrcId);

    //计算音频
    void buildFrequencySpectrum(QAudioBuffer buffer);

    QMediaPlayer *getPlayer() const;

    int getLoopType() const;
    void setLoopType(int newLoopType);

    QVector<double> getAllSamples() const;

    QAudioOutput *getAudioOutput() const;

    QList<LrcData *> getLrcList() const;

    LrcData *getPlayingLrc() const;

    int playingMusicId() const;

    QList<int> musicList() const;

signals:

    //下载歌词
    void downLrc(QString key, QString url);

    //清空数据以及播放列表
    void finishClearData();

    //重建播放列表
    void musicListBuild();

    //将歌曲添加到播放播放列表
    void musicListAppend(int, int);

    //绘制音频波形
    void cppDrawLine(QVector<double>);

    //歌词加载完成
    void lrcLoaded();

    // 清空歌词界面
    void clearLrc();

    void loopTypeChanged();

    void playingLrcIdChange();
    void playingMusicIdChanged();
    void musicListChanged();
};

#endif // MEDIAPLAYER_H
