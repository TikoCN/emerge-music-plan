#include "artist.h"

Artist::Artist(QString name)
    : name(name)
{

}

int Artist::getId() const
{
    return id;
}

QString Artist::getName() const
{
    return name;
}

QList<Music *> Artist::getMusicList() const
{
    return musicList;
}
