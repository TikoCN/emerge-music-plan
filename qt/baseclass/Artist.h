#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include "SortType.h"

using ArtistPtr = QSharedPointer<class Artist>;

class Artist {
public:
    int musicCount = 0; // 音乐条目总数
    QString name = QObject::tr("未知歌手");
    QString lineKey = "";
    long long duration = 0;
    int id = -1;
    int firstMusic = 0;
    SORT_TYPE sortType = SORT_TITTLE_ASC;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

#endif // ARTIST_H
