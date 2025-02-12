#ifndef HOSTTIME_H
#define HOSTTIME_H

#include "music.h"
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
    //加载文件夹中的音乐
    void working();

signals:
    //音乐加载完成
    void musicLoaded(QList<Music *>, QStringList);

    //完成任务
    void finishLoad(TaskCell* cell);
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

    int workPos = 0;//工作位置
    int workNumber = 0;
    QSemaphore* semaphore;

    QFileInfoList musicFileList;//所有文件列表
    QStringList musicKeyList;//音乐key列表
    QList<TaskCell *> taskCellList;//任务计算单元
    QList<Music *> coreList;//计算好的音乐核心

public slots:
    //生成加载单元列表
    void buildTaskCellList();

    //加载音乐文件资源
    void loadMusicFile(QStringList dirList);

    //遍历文件夹得到所有子文件
    QFileInfoList getMusicUrl(QString dirPath);

    //获得一次的任务量
    bool getInfoList(QFileInfoList *list);

    //获得加载好的音乐数据
    void getMusicCoreList(QList<Music *> coreList, QStringList musicKeyList);

    //子线程完成任务
    void cellFinishWork(TaskCell *cell);

    //下载封面
    void onLineGetCoverFromNetEase(QString key, QString url);

    void onLineGetCoverFromQQMusic(QString key, QString url);

    void onLineGetCoverFromBing(QString key, QString url);

    void onLineGetCoverFromBaidu(QString key, QString url);

    //下载歌词
    void onLineGetLrcFromNetEase(QString key, QString url);

    void onLineGetLrcFromQQMusic(QString key, QString url);

    //加载图片写入文件
    void onLineCoverToFile(QString url, QString file);

signals:
    //筛选音乐文件
    void musicListBuild(QFileInfoList);

    //音乐文件加载完成
    void musicsLoaded(QList<Music *>, QStringList);

    //开始工作
    void startWork();

};

#endif // HOSTTIME_H
