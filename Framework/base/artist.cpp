#include "artist.h"
#include "base.h"
#include "sqlite/sqlite.h"
Artist::Artist(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{

}

void Artist::setName(const QString &newName)
{
    if (name == newName)
        return;
    name = newName;
    emit nameChanged();

    SQLite::getInstance()->updateArtist(this);
}

long long Artist::getDuration() const
{
    return duration;
}

QList<int> Artist::getMusicList() const
{
    return musicList;
}

QString Artist::getLineKey() const
{
    return lineKey;
}

int Artist::getId() const
{
    return id;
}

QString Artist::getName() const
{
    return name;
}
