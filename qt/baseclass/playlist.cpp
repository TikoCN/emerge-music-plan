#include "playlist.h"
#include <QJsonObject>
#include "basetool/basetool.h"

PlayList::PlayList()
    : sort(SORT_TITTLE_ASC)
      , duration(0)
      , id(0)
      , isDir(false) {
}

QJsonObject PlayList::getJsonObject() const {
    QJsonObject json;
    json.insert("playList", name);
    json.insert("id", id);
    json.insert("isDir", isDir);
    json.insert("duration", duration);
    json.insert("musicList", TypeConversion::intListToString(musicList));
    json.insert("sort", static_cast<int>(sort));
    json.insert("url", url);
    return json;
}
