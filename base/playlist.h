#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

using PlayListPtr = QSharedPointer<class PlayList>;

class PlayList
{
public:
    explicit PlayList();
    enum SORT_TYPE {
        SORT_TITTLE_ASC, SORT_TITTLE_DESC,
        SORT_ATRIST_ASC, SORT_ATRIST_DESC,
        SORT_ALUMB_ASC, SORT_ALUMB_DESC,
        SORT_DURATION_ASC, SORT_DURATION_DESC,
        SORT_LAST_EDIT_TIME_ASC, SORT_LAST_EDIT_TIME_DESC,
        SORT_LEVEL_ASC, SORT_LEVEL_DESC,
        SORT_PLAY_NUMBER_ASC, SORT_PLAY_NUMBER_DESC
    } sort;

    QString name;//列表名
    QString url;//文件夹路径
    QList<int> musicList;//音乐库列表
    long long duraiton;
    int id;//列表id
    bool isDir;

    QJsonObject getJsonObject();


};
#endif // PLAYLIST_H
