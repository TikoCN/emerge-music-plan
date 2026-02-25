#include "Artist.h"
#include "basetool/basetool.h"
#include <QJsonObject>
#include <utility>

QJsonObject Artist::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("artist_id", id);
    json.insert("duration", duration);
    json.insert("musicCount", musicCount);
    json.insert("lineKey", lineKey);
    json.insert("sort", static_cast<int>(sortType));
    return json;
}
