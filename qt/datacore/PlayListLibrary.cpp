#include "PlayListLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "PlayListLibraryModel.h"

PlayListLibrary::PlayListLibrary() : m_model(new PlayListLibraryModel(this)), m_loader(new DataLoader(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildPlayListPlayer,
            this, &PlayListLibrary::buildPlayListPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &PlayListLibrary::finish);
    connect(m_loader, &DataLoader::loadInitData, this, &PlayListLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &PlayListLibrary::loadMoreByKey);
}

PlayListLibrary::~PlayListLibrary() {
    delete m_model;
    delete m_loader;
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

DataLoader *PlayListLibrary::loader() {
    return m_loader;
}

void PlayListLibrary::loadByKey(const QString &key) {
    Q_UNUSED(key);
    m_model->loadAll();
}

void PlayListLibrary::loadMoreByKey(int index, const QString &key) {
    Q_UNUSED(index);
    Q_UNUSED(key);
    // Playlist不需要分页加载
}