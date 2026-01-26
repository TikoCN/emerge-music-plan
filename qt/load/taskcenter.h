#ifndef TASKCENTER_H
#define TASKCENTER_H

#include <QObject>
#include <QFileInfoList>
#include <QThreadPool>
#include "baseclass/mediadata.h"

class TaskCenter : public QObject
{
    Q_OBJECT
private:
    static TaskCenter* instance;
    explicit TaskCenter();

    QThread *m_thread;
    QFileInfoList m_fileInfoList;
    QList<MediaData> m_dataList;
    QSet<QString> m_artistSet;
    QSet<QString> m_albumLSet;
    QSet<QString> m_playlistSet;
    QList<QPair<QString, QString>> m_artistMusicList;
    QList<QPair<QString, QString>> m_playlistMusicList;

    QThreadPool *m_pool;
    int m_work{};                      //工作单元数量

    // 写入数据库
    void writeDataSQL();

    //遍历文件夹得到所有子文件
    void filterFileInfo(const QStringList& dirPath);
    void filterFileInfo(const QFileInfoList& dir);

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
            delete instance;
    }

    ~TaskCenter() override;


    //删除数据
    void clearData();

    void start();

    void appendInfo(const QFileInfoList& fileInfoList);
    void appendMedia(QList<MediaData> dataList);
};


#endif // TASKCENTER_H
