#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

using PlayListPtr = QSharedPointer<class PlayList>;

class PlayList {
public:
    explicit PlayList();

    enum SORT_TYPE {
        SORT_TITTLE_ASC, SORT_TITTLE_DESC,
        SORT_ARTIST_ASC, SORT_ARTIST_DESC,
        SORT_ALBUM_ASC, SORT_ALBUM_DESC,
        SORT_DURATION_ASC, SORT_DURATION_DESC,
        SORT_LAST_EDIT_TIME_ASC, SORT_LAST_EDIT_TIME_DESC,
        SORT_LEVEL_ASC, SORT_LEVEL_DESC,
        SORT_PLAY_NUMBER_ASC, SORT_PLAY_NUMBER_DESC
    } sort;

    QString name; //列表名
    QString url; //文件夹路径
    QList<int> musicList; //音乐库列表
    long long duration;
    int id; //列表id
    bool isDir;

    [[nodiscard]] QJsonObject getJsonObject() const;
};
#endif // PLAYLIST_H
