#include "playlist.h"
#include <QUrl>
#include <QDesktopServices>
#include <QJsonObject>
#include "base.h"

PlayList::PlayList()
{
    sort = SORT_TITTLE_ASC;
}

QJsonObject PlayList::getJsonObject()
{
    QJsonObject json;
    json.insert("playList", name);
    json.insert("id", id);
    json.insert("isDir", isDir);
    json.insert("duration", duration);
    json.insert("musicList", Base::getInstance()->intListToString(musicList));
    json.insert("sort", static_cast<int>(sort));
    json.insert("url", url);
    return json;
}
