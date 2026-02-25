#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QSet>
#include <QSharedPointer>
#include "SortType.h"

using AlbumPtr = QSharedPointer<class Album>;

class Album{
public:
    int musicCount = 0; // 音乐条目总数
    QString name = QObject::tr("未知专辑");
    QString lineKey = "";
    QSet<QString> artistSet = {QObject::tr("未知歌手")};
    long long duration = 0;
    int id = -1;
    int firstMusic = 0;
    SORT_TYPE sortType = SORT_TITTLE_ASC;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

#endif // ALBUM_H
