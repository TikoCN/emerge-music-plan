#ifndef HOSTTIME_H
#define HOSTTIME_H

#include "music.h"
#include "table.h"
#include <QList>
#include <QFileInfoList>
#include <QThread>
#include <QSemaphore>
#include <QRegularExpression>

class TaskCell : public QObject
{
    Q_OBJECT
private:
    QThread* thread;
public:
    TaskCell();
    ~TaskCell();

    // 加载音乐核心数据
    void loadMusicCore();

    // 加载用户列表
    void loadUserTable();

signals:
    //音乐核心完成
    void loadedMusicCore(QList<Music *>);

    //音乐核心完成
    void loadedUserTable(QList<Music *>, int);

    //完成任务
    void finishMusicCore(TaskCell* cell);
    void finishUserTable(TaskCell* cell);
};

class HostTime : public QObject
{
    Q_OBJECT
private:
    static HostTime* instance;
    explicit HostTime();

public:
    static HostTime* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new HostTime;
        }
    }

    ~HostTime();

    int workPos = 0;                           // 正在工作的线程数
    int workNumber = 0;
    int tableId = 0;                           // 工作列表id
    int dirTableSize = 0;                      // 本地列表数量
    QSemaphore* semaphore;

    QFileInfoList musicFileList;               // 所有文件列表
    QStringList musicNameList;                 // 旧音乐名列表
    QList<QStringList> tableMusic;              // 列表歌曲名单
    QList<TaskCell *> taskCellList;            // 任务计算单元
    QList<Music *> musicList;                   // 计算好的音乐核心
    QList<Table *> tableList;                  // 播放列表

public slots:
    //生成加载单元列表
    void buildHostTime();

    //加载音乐文件资源
    void loadMusicFile();
    void loadUserTable();

    //遍历文件夹得到所有子文件
    QFileInfoList getMusicUrl(QString dirPath);

    // 获得加载的任务
    bool getInfoList(QFileInfoList *list);
    bool getUserTableTask(QStringList *musicNameList, int *aim);

    // 获得加载好的音乐数据
    void getMusicCoreList(QList<Music *> musicList);
    void getUserTableMusic(QList<Music *> musicList, int tableId);

    //子线程完成任务
    void finishMusicCore(TaskCell* cell);
    void finishUserTable(TaskCell* cell);

    //删除数据
    void clearData();
private:
    QThread *thread;

signals:
    //筛选音乐文件
    void musicListBuild(QFileInfoList);

    //音乐文件加载完成
    void musicsLoaded(QList<Music *>, QList<Table *>);

    //开始工作
    void startMusicCore();
    void startUserTable();

    //初始化数据
    void initData();
};

#endif // HOSTTIME_H
