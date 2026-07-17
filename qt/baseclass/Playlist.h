#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include "macro/ConstantProperty.h"

using PlayListPtr = QSharedPointer<class PlayList>;

class PlayList {
    Q_GADGET

public:
    QT_CONSTANT_PROPERTY(QString, name, Name) // 列表名，
    QT_CONSTANT_PROPERTY(QString, url, Url)   // 文件夹路径，默认值
    QT_CONSTANT_PROPERTY(long long, duration, Duration)
    QT_CONSTANT_PROPERTY(bool, isDir, IsDir)
    QT_CONSTANT_PROPERTY(bool, isShow, IsShow)
    QT_CONSTANT_PROPERTY(int, firstMusic, FirstMusic)
    QT_CONSTANT_PROPERTY(int, id, Id)                 // 列表id，
    QT_CONSTANT_PROPERTY(int, musicCount, MusicCount) // 音乐条目总数
    QT_CONSTANT_PROPERTY(int, sort, Sort)             // 默认值

    PlayList();
    [[nodiscard]] QJsonObject getJsonObject() const;
};

Q_DECLARE_METATYPE(PlayList)
#endif // PLAYLIST_H
