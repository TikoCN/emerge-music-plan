#ifndef MEDIADATA_H
#define MEDIADATA_H

#include <QString>
#include <QStringList>
#include <QObject>

struct MediaData {
    QString title = QObject::tr("未知歌曲");
    QString album = QObject::tr("未知专辑");
    QStringList artistList = {QObject::tr("未知歌手")};
    QString url = "";
    QString dir = "";
    long long duration = 0;
    bool isLove = false;
    int level = 0;
    int playNumber = 0;
};

#endif // MEDIADATA_H
