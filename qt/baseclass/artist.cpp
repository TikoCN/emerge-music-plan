#include "artist.h"
#include "basetool/basetool.h"
#include <QJsonObject>
#include <utility>

Artist::Artist(QString name, const int id, QString linekey)
    : name(std::move(name))
      , lineKey(std::move(linekey))
      , duration(0)
      , id(id)
      , isNoCover(false) {
}

QJsonObject Artist::getJsonObject() const {
    QJsonObject json;
    json.insert("artist", name);
    json.insert("artist_id", id);
    json.insert("duration", duration);
    json.insert("musicList", TypeConversion::intListToString(musicList));
    json.insert("lineKey", lineKey);
    return json;
}
