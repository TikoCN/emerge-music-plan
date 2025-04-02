#include "album.h"
#include "base.h"

Album::Album(QString name)
    : name(name)
{

}

QString Album::getLineKey() const
{
    return lineKey;
}

QString Album::getStringTime()
{
    long long time = 0;
    for (int i = 0; i < musicList.size(); ++i) {
        time += musicList[i]->endTime;
    }

    // 转换标准时间
    return Base::getInstance()->timeToString(time);
}

QString Album::getArtist()
{
    return ((QStringList)artistSet.values()).join(";");
}

int Album::getId() const
{
    return id;
}

QList<Music *> Album::getMusicList() const
{
    return musicList;
}

QString Album::getName() const
{
    return name;
}

