#include "album.h"
#include "base.h"
#include "sqlite/sqlite.h"
#include <QDir>

Album::Album(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{

}


QJsonObject Album::getJsonObject()
{
    QJsonObject json;
    json.insert("album", name);
    json.insert("album_id", id);
    json.insert("musicList", Base::getInstance()->intListToString(musicList));
    json.insert("lineKey", lineKey);
    json.insert("artistList", Base::getInstance()->stringSetToString(artistSet));
    json.insert("duration", duration);
    return json;
}

