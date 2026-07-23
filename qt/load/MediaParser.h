#ifndef MEDIAPARSER_H
#define MEDIAPARSER_H

#include <QObject>
#include <QRunnable>
#include <QFileInfoList>
#include "baseclass/MediaData.h"

class MediaParser : public QObject, public QRunnable {
    Q_OBJECT

private:
    QFileInfoList m_infoList;
    QList<MediaData> m_dataList;

public:
    explicit MediaParser(QFileInfoList infoList);

    void run() override;

signals:
    void parseFinished(QList<MediaData> dataList);
};

#endif // MEDIAPARSER_H