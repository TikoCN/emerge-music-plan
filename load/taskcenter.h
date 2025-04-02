#ifndef TASKCENTER_H
#define TASKCENTER_H

#include <QObject>
#include <QFileInfoList>
#include <QThreadPool>
#include "buildusertable.h"
#include "buildmusic.h"
#include "base/table.h"
#include "base/music.h"
#include "base/artist.h"
#include "base/album.h"
#include "QSemaphore"

class TaskCenter : public QObject
{
    Q_OBJECT
private:
    static TaskCenter* instance;
    explicit TaskCenter();

public:
    static TaskCenter* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new TaskCenter;
        }
    }

    ~TaskCenter();

    int workPos = 0;                           // 正在工作的线程数
    int workNumber = 0;
    int tableId = 0;                           // 工作列表id
    int dirTableSize = 0;                      // 本地列表数量
    QSemaphore* semaphore;

    QFileInfoList musicFileList;               // 所有文件列表
    QStringList musicNameList;                 // 旧音乐名列表
    QStringList keyList;                       // 字母对照表
    QList<QStringList> tableMusic;              // 列表歌曲名单
    QList<BuildMusic *> buildMusicList;            // 任务计算单元
    QList<BuildUserTable *> buildUserTableList;            // 任务计算单元
    QList<Music *> musicList;                   // 计算好的音乐核心
    QList<Table *> tableList;                  // 播放列表
    QList<Artist *> artistList;                // 歌手列表
    QList<Album *> albumList;                  // 专辑列表
    QList<QStringList> chinesePinyin;           // 中文拼音对照表

public slots:
    //生成加载单元列表
    void buildTaskCenter();

    //加载音乐文件资源
    void loadMusicFile();
    void loadUserTable();

    int getEnglishId(QString name);
    void insertArtist(Music *music);
    void insertAlbum(Music *music);

    //遍历文件夹得到所有子文件
    void filterFileInfo(QStringList dirPath);
    void filterFileInfo(QFileInfoList dir);

    // 获得加载的任务
    bool getInfoList(QFileInfoList *list);
    bool getUserTableTask(QStringList *musicNameList, int *aim);

    // 获得加载好的音乐数据
    void getMusicCoreList(QList<Music *> musicList, QList<QList<Music *>> tableMusicList);
    void getUserTableMusic(QList<Music *> musicList, int tableId);

    //子线程完成任务
    void finishMusicCore();
    void finishUserTable();

    //删除数据
    void clearData();
private:
    QThread *thread;

signals:
    //筛选音乐文件
    void musicListBuild(QFileInfoList);

    //音乐文件加载完成
    void musicsLoaded(QList<Music *>, QList<Table *>, QList<Artist *>, QList<Album *>);

    //开始工作
    void startMusicCore();
    void startUserTable();

    //初始化数据
    void initData();
};


#endif // TASKCENTER_H
