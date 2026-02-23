#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMediaPlayer>
#include <QPixmap>
#include "mediaplay/lrcdatacontrol.h"
#include "sqlite/sqlite.h"

class MediaPlayer :public LrcDataControl
{
    Q_OBJECT
private:
    static MediaPlayer* instance;
    MediaPlayer(BaseTool *baseTool, DataActive *dataActive, TLog *log, SQLite *sql, QObject *parent = nullptr);

    int m_loopType;//播放种类

    QList<int> m_musicList;//正在播放列表
    int m_PlayingMusicListId;//正在播放歌曲的列表id
    int m_playingMusicId;// 正在播放音乐id
    MusicPtr m_playingMusic;
    SQLite *m_sqlite;

    Q_PROPERTY(int playingMusicId READ playingMusicId CONSTANT)
    Q_PROPERTY(int loopType READ getLoopType WRITE setLoopType NOTIFY loopTypeChanged FINAL)

public:
    static MediaPlayer* getInstance(){
        return instance;
    }

    static void buildInstance(BaseTool *baseTool, DataActive *dataActive, TLog *log, SQLite *sql) {
        if(instance == nullptr){
            instance = new MediaPlayer(baseTool, dataActive, log, sql);
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
    Q_INVOKABLE void playMusicByListId(int musicListId);

    //播放音乐
    void buildPlayingList(QList<int> list, int playMusicId = 0);
    void buildPlayingId(int musicId);
    Q_INVOKABLE void buildPlayingArtist(int artistId, int listId = 0);
    Q_INVOKABLE void buildPlayingAlbum(int albumId, int listId = 0);
    Q_INVOKABLE void buildPlayingPlayList(int playListId, int listId = 0);

    // 播放列表插入歌曲
    void insertPlayingList(const QList<int>& list);
    void insertPlayingId(int musicId);
    Q_INVOKABLE void insertPlayingArtist(int artistId);
    Q_INVOKABLE void insertPlayingAlbum(int albumId);
    Q_INVOKABLE void insertPlayingPlayList(int playListId);

    void appendPlayingList(const QList<int>& list);
    void appendPlayingId(int musicId);
    Q_INVOKABLE void appendPlayingArtist(int artistId);
    Q_INVOKABLE void appendPlayingAlbum(int albumId);
    Q_INVOKABLE void appendPlayingPlayList(int playListId);

    [[nodiscard]] int playingMusicId() const;
    Q_INVOKABLE [[nodiscard]] QList<int> getMusicList(int size, int start) const;

    Q_INVOKABLE void initData();
    void setPlayingMusicListId(int playingMusicListId);
    [[nodiscard]] int getPlayingMusicListId() const;
signals:

    //重建播放列表
    void musicListBuild();
    //将歌曲添加到播放播放列表
    void musicListAppend(int, int);

    void loopTypeChanged();
};

#endif // MEDIAPLAYER_H
