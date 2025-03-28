#include "alumb.h"
#include "base.h"

Alumb::Alumb(QString name)
    : name(name)
{

}

QString Alumb::getLineKey() const
{
    return lineKey;
}

QString Alumb::getStringTime()
{
    long long time = 0;
    for (int i = 0; i < musicList.size(); ++i) {
        time += musicList[i]->endTime;
    }

    // 转换标准时间
    return Base::getInstance()->timeToString(time);
}

QString Alumb::getArtist()
{
    return ((QStringList)artistSet.values()).join(";");
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

