#ifndef MEDIADATA_H
#define MEDIADATA_H

#include <QString>
#include <QObject>

struct MediaData {
    QString   title      = QObject::tr("未知歌曲");
    QString   album      = QObject::tr("未知专辑");
    QString   artist     = QObject::tr("未知歌手");
    QString   url        = "";
    QString   dir        = "";
    long long duration   = 0;
    bool      isLove     = false;
    int       level      = 0;
    int       playNumber = 0;
};

#endif // MEDIADATA_H
