#ifndef TASKCENTER_H
#define TASKCENTER_H

#include <QObject>
#include <QFileInfoList>
#include <QThreadPool>
#include "base/mediadata.h"

class TaskCenter : public QObject
{
    Q_OBJECT
private:
    static TaskCenter* instance;
    explicit TaskCenter();

    QThread *_thread;
    QFileInfoList _fileInfoList;
    QList<MediaData> _dataList;
    QSet<QString> _artistSet;
    QSet<QString> _albumLSet;
    QSet<QString> _playlistSet;
    QList<QPair<QString, QString>> _artistMusicList;
    QList<QPair<QString, QString>> _playlistMusicList;

    QThreadPool *_pool;
    int _work;                      //工作单元数量

    // 写入数据库
    void writeDataSQL();

    //遍历文件夹得到所有子文件
    void filterFileInfo(QStringList dirPath);
    void filterFileInfo(QFileInfoList dir);

    void selectFile();
    void loadMedia();

public:
    static TaskCenter* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new TaskCenter;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    ~TaskCenter();


    //删除数据
    void clearData();

    void start();

    void appendInfo(QFileInfoList fileInfoList);
    void appendMedia(QList<MediaData> dataList);
};


#endif // TASKCENTER_H
