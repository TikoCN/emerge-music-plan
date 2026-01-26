#include "album.h"
#include "basetool/basetool.h"
#include <QJsonObject>
#include <QDir>
#include <utility>

Album::Album(QString name, const int id, QString lineKey)
    : name(std::move(name))
      , lineKey(std::move(lineKey))
      , duration(0)
      , id(id) {
}


QJsonObject Album::getJsonObject() const {
    QJsonObject json;
    json.insert("album", name);
    json.insert("album_id", id);
    json.insert("musicList", TypeConversion::intListToString(musicList));
    json.insert("lineKey", lineKey);
    json.insert("artistList", TypeConversion::stringListToString(artistSet.values()));
    json.insert("duration", duration);
    return json;
}

