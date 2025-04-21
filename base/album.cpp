#include "album.h"
#include "base.h"
#include "sqlite/sqlite.h"
#include <QDir>

Album::Album(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{

}

void Album::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();

    SQLite::getInstance()->updateAlbum(this);
}

long long Album::getDuration() const
{
    return duration;
}

QList<int> Album::getMusicList() const
{
    return musicList;
}

QString Album::getLineKey() const
{
    return lineKey;
}

QString Album::getArtist()
{
    return ((QStringList)artistSet.values()).join(";");
}

int Album::getId() const
{
    return id;
}

QString Album::getName() const
{
    return name;
}

