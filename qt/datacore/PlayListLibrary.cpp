#include "PlayListLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "PlayListLibraryModel.h"

PlayListLibrary::PlayListLibrary() : m_model(new PlayListLibraryModel(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildPlayListPlayer,
            this, &PlayListLibrary::buildPlayListPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &PlayListLibrary::finish);
}

PlayListLibrary::~PlayListLibrary() {
    delete m_model;
}

void PlayListLibrary::appendPlayList(const QString &name) {
    DataActive::appendPlayList(name);
}

void PlayListLibrary::updatePlayListName(const int playListId, const QString &name) {
    DataActive::getInstance().updatePlayListName(playListId, name);
}

void PlayListLibrary::updatePlayListSort(const int playListId, const int sort) {
    DataActive::getInstance().updatePlayListSort(playListId, sort);
}

PlayList PlayListLibrary::getPlayListData(const int id) {
    return DataActive::getInstance().getPlayListData(id);
}

QJsonObject PlayListLibrary::getPlayListJson(const int id) {
    return DataActive::getInstance().getPlayListJson(id);
}

int PlayListLibrary::checkPlayListName(const QString &name) const {
    return SQLite::getInstance().checkPlayListName(name);
}

bool PlayListLibrary::movePlayListMusic(const QString &playListName, const QString &playListNameNew) const {
    return SQLite::getInstance().movePlayListMusic(playListName, playListNameNew);
}

bool PlayListLibrary::addPlayListMusicToPlayList(const QString &sourcePlayListName, const QString &targetPlayListName) const {
    return SQLite::getInstance().addPlayListMusicToPlayList(sourcePlayListName, targetPlayListName);
}

QString PlayListLibrary::getAllList() const {
    return SQLite::getInstance().getAllList();
}

void PlayListLibrary::clearNullItem() {
    DataActive::getInstance().clearNullItem();
}

void PlayListLibrary::updateALLNameKey() const {
    DataActive::getInstance().updateALLNameKey();
}

PlayListLibraryModel *PlayListLibrary::model() {
    return m_model;
}