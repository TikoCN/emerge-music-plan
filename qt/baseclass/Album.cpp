#include "baseclass/Album.h"
#include "basetool/TypeConversion.h"
#include <QJsonObject>

Album::Album()
    : duration(0),
      firstMusic(0),
      musicCount(0),
      id(0),
      sort(0) {
}

QJsonObject Album::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("album_id", id);
    json.insert("musicCount", musicCount);
    json.insert("nameKey", nameKey);
    json.insert("duration", duration);
    json.insert("sort", sort);
    return json;
}
