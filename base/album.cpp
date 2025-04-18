#include "album.h"
#include "base.h"

Album::Album(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{

}

QList<int> Album::getMusicList() const
{
    return musicList;
}

QString Album::getLineKey() const
{
    return lineKey;
}

QString Album::getStringTime()
{
    long long time = 0;
    for (int i = 0; i < musicList.size(); ++i) {
        //
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

QString Album::getName() const
{
    return name;
}

