#include "album.h"
#include "basetool/basetool.h"
#include <QJsonObject>
#include <QDir>

QJsonObject Album::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("album_id", id);
    json.insert("musicCount", musicCount);
    json.insert("lineKey", lineKey);
    json.insert("artistList", TypeConversion::stringListToString(artistSet.values()));
    json.insert("duration", duration);
    json.insert("sort", static_cast<int>(sortType));
    return json;
}

