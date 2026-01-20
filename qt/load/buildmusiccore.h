#ifndef BUILDMUSICCORE_H
#define BUILDMUSICCORE_H

#include <QObject>
#include <QRunnable>
#include <QFileInfo>
#include "baseclass/mediadata.h"

class BuildMusicCore : public QObject, public QRunnable
{
    Q_OBJECT
private:
    QFileInfoList _infoList;
    QList<MediaData> _musicList;

public:
    explicit BuildMusicCore(QFileInfoList infoList);

    void run() override;

signals:
    void dataLoaded(QList<MediaData>);
};

#endif // BUILDMUSICCORE_H
