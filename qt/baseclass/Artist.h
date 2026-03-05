#ifndef ARTIST_H
#define ARTIST_H

#include <QObject>
#include "SortType.h"
#include "macro/ConstantProperty.h"

using ArtistPtr = QSharedPointer<class Artist>;

class Artist {
    Q_GADGET
public:
    QT_CONSTANT_PROPERTY(QString, name, Name)
    QT_CONSTANT_PROPERTY(QString, nameKey, NameKey)
    QT_CONSTANT_PROPERTY(long long, duration, Duration)
    QT_CONSTANT_PROPERTY(int, firstMusic, FirstMusic)
    QT_CONSTANT_PROPERTY(int, musicCount, MusicCount)
    QT_CONSTANT_PROPERTY(int, id, Id)
    QT_CONSTANT_PROPERTY(int, sort, Sort)

    Artist();
    [[nodiscard]] QJsonObject getJsonObject() const;
};

Q_DECLARE_METATYPE(Artist)

#endif // ARTIST_H
