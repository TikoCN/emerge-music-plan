#ifndef TASKCENTER_H
#define TASKCENTER_H

#include <atomic>
#include <QObject>
#include <QFileInfoList>
#include <QThreadPool>
#include <QSet>
#include "baseclass/MediaData.h"

class TaskCenter : public QObject {
    Q_OBJECT

private:
    static constexpr int BATCH_SIZE_LOAD  = 20;
    static constexpr int BATCH_SIZE_CHECK = 50;

    explicit TaskCenter();

    QFileInfoList    m_fileInfoList;
    QList<MediaData> m_dataList;
    QList<int>       m_allInvalidIds;

    QThreadPool *    m_pool;
    std::atomic<int> m_work;

    void loadMedia();
    void scanSubDirectories(const QStringList &subDirPaths);

public:
    static TaskCenter &getInstance() {
        static TaskCenter instance;
        return instance;
    }

    ~TaskCenter() override;

    void clearData();

    void startLoadFile();
    void startBootSequence();

    void onScanFinished(QFileInfoList fileInfoList, QStringList subDirPaths);
    void onParseFinished(QList<MediaData> dataList);

public slots:
    void checkFileExist(const QList<QPair<int, QString> > &musicDataList);
    void onInvalidDataCleared();

signals:
    void fileCheckFinished(const QList<int> &invalidMusicIds);
};

#endif // TASKCENTER_H
