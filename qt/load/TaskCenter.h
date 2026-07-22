#ifndef TASKCENTER_H
#define TASKCENTER_H

#include <atomic>
#include <QObject>
#include <QFileInfoList>
#include <QThreadPool>
#include "baseclass/MediaData.h"

class TaskCenter : public QObject
{
    Q_OBJECT
private:
    // 每批次处理的文件数量
    static constexpr int BATCH_SIZE_SELECT = 20;
    static constexpr int BATCH_SIZE_LOAD   = 20;
    static constexpr int BATCH_SIZE_CHECK  = 50;

    explicit TaskCenter();

    QFileInfoList m_fileInfoList;
    QList<MediaData> m_dataList;
    QSet<QString> m_artistSet;
    QSet<QString> m_albumSet;
    QSet<QString> m_playlistSet;
    QList<QPair<QString, QString>> m_artistMusicList;
    QList<QPair<QString, QString>> m_albumMusicList;
    QList<QPair<QString, QString>> m_playlistMusicList;

    QThreadPool *m_pool;
    std::atomic<int> m_work; // 工作单元数量，原子操作保证线程安全

    void writeDataSQL();

    // 使用迭代式遍历，避免深层递归导致栈溢出
    void filterFileInfo(const QStringList& dirPath);

    void selectFile();
    void loadMedia();

public:
    static TaskCenter& getInstance(){
        static TaskCenter instance;
        return instance;
    }

    ~TaskCenter() override;

    void clearData();

    void start();

    void appendInfo(const QFileInfoList& fileInfoList);
    void appendMedia(const QList<MediaData>& dataList);

public slots:
    void checkFileExist(const QList<QPair<int, QString>> &musicDataList);

signals:
    void fileCheckFinished(const QList<int> &invalidMusicIds);
};


#endif // TASKCENTER_H
