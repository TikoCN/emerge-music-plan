#include "Playlist.h"
#include <QJsonObject>
#include "basetool/basetool.h"

PlayList::PlayList()
    : duration(0),
      isDir(false),
      isShow(false),
      firstMusic(0),
      id(0),
      musicCount(0),
      sort(0) {
}

QJsonObject PlayList::getJsonObject() const {
    QJsonObject json;
    json.insert("name", name);
    json.insert("id", id);
    json.insert("isDir", isDir);
    json.insert("duration", duration);
    json.insert("musicCount", musicCount);
    json.insert("sort", sort);
    json.insert("url", url);
    return json;
}
