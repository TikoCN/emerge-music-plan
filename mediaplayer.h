#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "musiccore.h"
#include "lrcdata.h"
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

    Q_PROPERTY(QMediaPlayer *player READ getPlayer CONSTANT)

    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

    Q_PROPERTY(QVector<double> allSamples READ getAllSamples CONSTANT)

    Q_PROPERTY(QAudioOutput *audioOutput READ getAudioOutput CONSTANT)

    Q_PROPERTY(QList<Music *> musicList READ getMusicList CONSTANT)

    Q_PROPERTY(QList<LrcData *> lrcList READ getLrcList CONSTANT)

    Q_PROPERTY(Music *playingMusic READ getPlayingMusic CONSTANT)

    Q_PROPERTY(LrcData *playingLrc READ getPlayingLrc CONSTANT)

public:
    static MediaPlayer* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new MediaPlayer;
        }
    }

    QMediaPlayer *player;//播放设备
    QAudioOutput *audioOutput;//音频输出
    QAudioBufferOutput *bufferOutput;//缓冲区输出

    QList<Music *> musicList;//正在播放列表
    QVector<double> allSamples;//处理之后的音乐样本
    QList<LrcData *> lrcList;//歌词

    MusicCore *core;//音乐数据中心
    int loopType;//播放种类
    int playingMusicListId;//正在播放歌曲的列表id
    Music *playingMusic;// 正在播放音乐
    LrcData *playingLrc;// 正在播放歌词行

public:
    //删除以及加载的数据
    Q_INVOKABLE void clearData();

    //更新播放设备
    void updateAudioOutPut();

    //播放音乐
    Q_INVOKABLE void playMusic(int table, int music);

    //加载歌词
    void loadLrcList();

    //将歌曲添加到正在播放
    Q_INVOKABLE void musicInsertPlayingTable(int coreId);

    //将歌词添加到播放下一首
    Q_INVOKABLE void playingInsertMusic(int coreId);

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

    QList<Music *> getMusicList() const;

    QAudioOutput *getAudioOutput() const;

    QList<LrcData *> getLrcList() const;

    Music *getPlayingMusic() const;

    LrcData *getPlayingLrc() const;

signals:

    //下载歌词
    void downLrc(QString key, QString url);

    //清空数据以及播放列表
    void finishClearData();

    //重建播放列表
    void cppBuildPlayingTable();

    //将歌曲添加到正在播放
    void cppMusicInsertPlayingTable(int coreId);

    //将歌词添加到播放下一首
    void cppPlayingInsertMusic(int musicId);

    //绘制音频波形
    void cppDrawLine(QVector<double>);

    //歌词加载完成
    void lrcLoaded();

    void loopTypeChanged();

    void playingLrcIdChange();
};

#endif // MEDIAPLAYER_H
