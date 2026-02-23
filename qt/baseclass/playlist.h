#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include "SortType.h"

using PlayListPtr = QSharedPointer<class PlayList>;

class PlayList {
public:
    QString name = QObject::tr("新建列表"); //列表名
    QString url = ""; //文件夹路径
    long long duration = 0;
    int id = -1; //列表id
    int musicConut = 0; // 音乐条目总数
    bool isDir = false;
    bool isShow = true;
    int firstMusic = 0;
    SORT_TYPE sortType = SORT_TITTLE_ASC;

    [[nodiscard]] QJsonObject getJsonObject() const;
};
#endif // PLAYLIST_H
