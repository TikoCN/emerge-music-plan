#include "alumb.h"

Alumb::Alumb(QString name)
    : name(name)
{

}

int Alumb::getId() const
{
    return id;
}

QList<Music *> Alumb::getMusicList() const
{
    return musicList;
}

QString Alumb::getName() const
{
    return name;
}

