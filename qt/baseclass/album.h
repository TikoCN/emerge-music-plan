#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QSet>
#include <QSharedPointer>
using AlbumPtr = QSharedPointer<class Album>;

class Album{
public:
    explicit Album(QString name, int id, QString lineKey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    QSet<QString> artistSet;
    long long duration;
    int id;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

#endif // ALBUM_H
