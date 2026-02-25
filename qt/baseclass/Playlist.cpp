#include "Playlist.h"
#include <QJsonObject>
#include "basetool/basetool.h"

QJsonObject PlayList::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("id", id);
    json.insert("isDir", isDir);
    json.insert("duration", duration);
    json.insert("musicCount", musicConut);
    json.insert("sort", static_cast<int>(sortType));
    json.insert("url", url);
    return json;
}
