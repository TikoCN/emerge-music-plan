#include "artist.h"
#include "basetool/basetool.h"
#include <QJsonObject>

Artist::Artist(QString name, int id, QString lineKey)
    : name(name), id(id), lineKey(lineKey)
{
}

QJsonObject Artist::getJsonObject()
{
    QJsonObject json;
    json.insert("artist", name);
    json.insert("artist_id", id);
    json.insert("duration", duration);
    json.insert("musicList", BaseTool::getInstance()->getTypeConversion()->intListToString(musicList));
    json.insert("lineKey", lineKey);
    return json;
}
