#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
using ArtistPtr = QSharedPointer<class Artist>;

class Artist {
public:
    explicit Artist(QString name, int id, QString linekey);

    QList<int> musicList; // 音乐列表
    QString name;
    QString lineKey;
    long long duration;
    int id;
    bool isNoCover;

    [[nodiscard]] QJsonObject getJsonObject() const;
};

#endif // ARTIST_H
