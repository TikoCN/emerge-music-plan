#include "Artist.h"
#include "basetool/basetool.h"
#include <QJsonObject>

Artist::Artist()
    : duration(0),
      firstMusic(0),
      musicCount(0),
      id(0),
      sort(0) {
}

QJsonObject Artist::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("artist_id", id);
    json.insert("duration", duration);
    json.insert("musicCount", musicCount);
    json.insert("nameKey", nameKey);
    json.insert("sort", sort);
    return json;
}
