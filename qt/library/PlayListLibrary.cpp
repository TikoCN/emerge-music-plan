#include "PlayListLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/PlaylistModel.h"

void PlayListLibrary::appendUser(const QString &name) {
    if (SQLite::getInstance().playListRepository.appendUser(name)) {
    }
}

void PlayListLibrary::updateName(const int playListId, const QString &name) {
    DataActive::getInstance().updatePlayListName(playListId, name);
}

void PlayListLibrary::updateSort(const int playListId, const int sort) {
    DataActive::getInstance().updatePlayListSort(playListId, sort);
}

PlayList PlayListLibrary::getData(const int id) {
    return DataActive::getInstance().getPlayListData(id);
}

QJsonObject PlayListLibrary::getJson(const int id) {
    return DataActive::getInstance().getPlayListJson(id);
}

int PlayListLibrary::allowName(const QString &name) const {
    return SQLite::getInstance().playListRepository.allowName(name);
}

bool PlayListLibrary::moveMusic(const QString &playListName, const QString &playListNameNew) const {
    return SQLite::getInstance().playListRepository.moveMusic(playListName, playListNameNew);
}

bool PlayListLibrary::addMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const {
    return SQLite::getInstance().playListRepository.addMusicToPlayList(sourcePlayListName, targetPlayListName);
}

void PlayListLibrary::clearNullItem() {
    DataActive::getInstance().clearNullItem();
}

void PlayListLibrary::updateALLNameKey() const {
    DataActive::getInstance().updateALLNameKey();
}
