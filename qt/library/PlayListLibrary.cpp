#include "PlayListLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/PlayListLibraryModel.h"

PlayListLibrary::PlayListLibrary()
    : m_model(new PlayListLibraryModel(this)),
      m_loader(new DataLoader(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildPlayListPlayer,
            this, &PlayListLibrary::buildPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &PlayListLibrary::finish);
    connect(m_loader, &DataLoader::loadInitData, this, &PlayListLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &PlayListLibrary::loadMoreByKey);
}

PlayListLibrary::~PlayListLibrary() {
    delete m_model;
    delete m_loader;
}

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

QString PlayListLibrary::getAllList() const {
    return SQLite::getInstance().playListRepository.getAllList();
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
