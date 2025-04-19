#include "artist.h"
#include "base.h"

Artist::Artist(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{

}

QList<int> Artist::getMusicList() const
{
    return musicList;
}

QString Artist::getLineKey() const
{
    return lineKey;
}

QString Artist::getStringTime()
{
    long long time = 0;
    for (int i = 0; i < musicList.size(); ++i) {

    }

    // 转换标准时间
    return Base::getInstance()->timeToString(time);
}

int Artist::getId() const
{
    return id;
}

QString Artist::getName() const
{
    return name;
}
