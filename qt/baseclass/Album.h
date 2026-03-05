#ifndef ALBUM_H
#define ALBUM_H

#include <QObject>
#include <QSharedPointer>
#include "macro/ConstantProperty.h"

using AlbumPtr = QSharedPointer<class Album>;

class Album{
    Q_GADGET
public:
    QT_CONSTANT_PROPERTY(QString, name, Name)
    QT_CONSTANT_PROPERTY(QString, nameKey, NameKey)
    QT_CONSTANT_PROPERTY(long long, duration, Duration)
    QT_CONSTANT_PROPERTY(int, firstMusic, FirstMusic)
    QT_CONSTANT_PROPERTY(int, musicCount, MusicCount)
    QT_CONSTANT_PROPERTY(int, id, Id)
    QT_CONSTANT_PROPERTY(int, sort, Sort)

    Album();
    [[nodiscard]] QJsonObject getJsonObject() const;
};

Q_DECLARE_METATYPE(Album)
#endif // ALBUM_H
