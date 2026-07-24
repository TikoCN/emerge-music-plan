#include "AlbumLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/AlbumLibraryModel.h"

AlbumLibrary::AlbumLibrary()
    : m_model(new AlbumLibraryModel(this)),
      m_loader(new DataLoader(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildAlbumPlayer,
            this, &AlbumLibrary::buildPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &AlbumLibrary::finish);
    connect(m_loader, &DataLoader::loadInitData, this, &AlbumLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &AlbumLibrary::loadMoreByKey);
}

AlbumLibrary::~AlbumLibrary() {
    delete m_model;
    delete m_loader;
}

void AlbumLibrary::updateName(const int albumId, const QString &name) {
    DataActive::getInstance().updateAlbumName(albumId, name);
}

void AlbumLibrary::updateSort(const int albumId, const int sort) {
    DataActive::getInstance().updateAlbumSort(albumId, sort);
}

Album AlbumLibrary::getData(const int id) {
    return DataActive::getInstance().getAlbumData(id);
}

QJsonObject AlbumLibrary::getJson(const int id) {
    return DataActive::getInstance().getAlbumJson(id);
}

QStringList AlbumLibrary::getKeys() const {
    return SQLite::getInstance().albumRepository.getKeys();
}

bool AlbumLibrary::moveMusic(const QString &albumName, const QString &albumNameNew) const {
    return SQLite::getInstance().albumRepository.moveMusic(albumName, albumNameNew);
}

bool AlbumLibrary::addMusicToPlayList(const QString &albumName, const QString &playListName) const {
    return SQLite::getInstance().albumRepository.addMusicToPlayList(albumName, playListName);
}

AlbumLibraryModel *AlbumLibrary::model() {
    return m_model;
}

DataLoader *AlbumLibrary::loader() {
    return m_loader;
}

void AlbumLibrary::loadByKey(const QString &key) {
    m_model->loadByKey(key, m_loader->getLoadSize(), 0);
}

void AlbumLibrary::loadMoreByKey(int index, const QString &key) {
    m_model->loadMoreByKey(key, m_loader->getLoadSize(), index);
}
