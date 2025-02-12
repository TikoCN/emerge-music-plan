#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include "table.h"
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

    Q_PROPERTY(QList<Table *> tableList READ getTableList WRITE setTableList NOTIFY tableListChanged FINAL)

    Q_PROPERTY(QMediaPlayer *player READ getPlayer WRITE setPlayer NOTIFY playerChanged FINAL)

    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

    Q_PROPERTY(Music *playingCore READ getPlayingCore WRITE setPlayingCore NOTIFY playingCoreChanged FINAL)

    Q_PROPERTY(QList<LrcData *> lrcList READ getLrcList WRITE setLrcList NOTIFY lrcListChanged FINAL)

    Q_PROPERTY(QList<Music *> musicList READ getMusicList WRITE setMusicList NOTIFY musicListChanged FINAL)

    Q_PROPERTY(QList<Music *> coreList READ getCoreList WRITE setCoreList NOTIFY coreListChanged FINAL)

    Q_PROPERTY(QVector<double> allSamples READ getAllSamples WRITE setAllSamples NOTIFY allSamplesChanged FINAL)

    Q_PROPERTY(QAudioOutput *audioOutput READ getAudioOutput WRITE setAudioOutput NOTIFY audioOutputChanged FINAL)

    Q_PROPERTY(LrcData *playedLrc READ getPlayedLrc WRITE setPlayedLrc NOTIFY playedLrcChanged FINAL)

    Q_PROPERTY(LrcData *playingLrc READ getPlayingLrc WRITE setPlayingLrc NOTIFY playingLrcChanged FINAL)

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

    QList<Table *> tableList;//播放列表
    QList<Music *> coreList;//音乐核心列表
    QList<LrcData *> lrcList;//歌词
    QList<Music *> musicList;//正在播放列表
    QVector<double> allSamples;//处理之后的音乐样本

    Music *playingCore;//正在播放核心
    LrcData* playingLrc;//正在播放歌词
    LrcData* playedLrc;//即将播放歌词

    int playingMusic;//正在播放音乐id
    int loopType;//播放种类

public:
    //获得音乐核心
    void getMusicCoreAndCover(QList<Music *>musicList, QStringList musicKeyList);

    //新建播放列表
    Q_INVOKABLE void addTable(QString tableName, bool isDir = false);

    //读取自建列表
    void buildNoDirTable(QStringList musicKeyList);

    //将歌曲移动到
    Q_INVOKABLE void tableMoveMusic(int orgTableId, int musicId, int aimTalbeId);

    //更新播放设备
    void updateAudioOutPut();

    //播放音乐
    Q_INVOKABLE void playMusic(int table, int music);

    //将歌曲添加到正在播放
    Q_INVOKABLE void musicInsertPlayingTable(int coreId);

    //将歌词添加到播放下一首
    Q_INVOKABLE void playingInsertMusic(int coreId);

    //加载歌词
    void loadLrcList(QString lrcUrl, qint64 endTime);

    //选择当前播放歌词
    void selectPlayLrc(qint64 time);

    //跳转到目标歌词
    Q_INVOKABLE void turnToLrc(int lrcId);

    //播放下一首
    Q_INVOKABLE void playNext(int forward);

    //获得时间文本
    Q_INVOKABLE QString getTimeString();

    //计算音频
    void buildFrequencySpectrum(QAudioBuffer buffer);

    QList<Table*> getTableList() const;
    void setTableList(const QList<Table*> &newTableList);

    QMediaPlayer *getPlayer() const;
    void setPlayer(QMediaPlayer *newPlayer);

    int getLoopType() const;
    void setLoopType(int newLoopType);

    Music *getPlayingCore() const;
    void setPlayingCore(Music *newPlayingCore);

    QList<LrcData *> getLrcList() const;
    void setLrcList(const QList<LrcData *> &newLrcList);

    QList<Music *> getMusicList() const;
    void setMusicList(const QList<Music *> &newMusicList);

    QList<Music *> getCoreList() const;
    void setCoreList(const QList<Music *> &newCoreList);

    QVector<double> getAllSamples() const;
    void setAllSamples(const QVector<double> &newAllSamples);

    QAudioOutput *getAudioOutput() const;
    void setAudioOutput(QAudioOutput *newAudioOutput);

    LrcData *getPlayedLrc() const;
    void setPlayedLrc(LrcData *newPlayedLrc);

    LrcData *getPlayingLrc() const;
    void setPlayingLrc(LrcData *newPlayingLrc);

signals:
    //新建本地列表
    void cppAddDirTable(int tableId);

    //新建用户列表
    void cppAddUserTable(int tableId);

    //歌词加载完成
    void cppLrcLoaded(int size);

    //重建播放列表
    void cppBuildPlayingTable();

    //将歌曲添加到正在播放
    void cppMusicInsertPlayingTable(int coreId);

    //将歌词添加到播放下一首
    void cppPlayingInsertMusic(int musicId);

    //绘制音频波形
    void cppDrawLine();

    //正在播放歌词改变
    void cppPlayingLrc();

    void tableListChanged();

    void playerChanged();

    void loopTypeChanged();

    void playingCoreChanged();
    void lrcListChanged();
    void musicListChanged();
    void coreListChanged();
    void allSamplesChanged();
    void audioOutputChanged();
    void playedLrcChanged();
    void playingLrcChanged();
};

#endif // MEDIAPLAYER_H
