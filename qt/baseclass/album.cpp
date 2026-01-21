#include "album.h"
#include "basetool/basetool.h"
#include <QJsonObject>
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
    json.insert("musicList", BaseTool::getInstance()->getTypeConversion()->intListToString(musicList));
    json.insert("lineKey", lineKey);
    json.insert("artistList", BaseTool::getInstance()->getTypeConversion()->stringListToString(artistSet.values()));
    json.insert("duration", duration);
    return json;
}

