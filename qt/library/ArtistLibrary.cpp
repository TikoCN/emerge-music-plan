#include "ArtistLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/ArtistLibraryModel.h"

ArtistLibrary::ArtistLibrary()
    : m_model(new ArtistLibraryModel(this)),
      m_loader(new DataLoader(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildArtistPlayer,
            this, &ArtistLibrary::buildPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &ArtistLibrary::finish);
    connect(m_loader, &DataLoader::loadInitData, this, &ArtistLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &ArtistLibrary::loadMoreByKey);
}

ArtistLibrary::~ArtistLibrary() {
    delete m_model;
    delete m_loader;
}

void ArtistLibrary::updateName(const int artistId, const QString &name) {
    DataActive::getInstance().updateArtistName(artistId, name);
}

void ArtistLibrary::updateSort(const int artistId, const int sort) {
    DataActive::getInstance().updateArtistSort(artistId, sort);
}

Artist ArtistLibrary::getData(const int id) {
    return DataActive::getInstance().getArtistData(id);
}

QJsonObject ArtistLibrary::getJson(const int id) {
    return DataActive::getInstance().getArtistJson(id);
}

QStringList ArtistLibrary::getKeys() const {
    return SQLite::getInstance().artistRepository.getKeys();
}

bool ArtistLibrary::moveMusic(const QString &artistName, const QString &artistNameNew) const {
    return SQLite::getInstance().artistRepository.moveMusic(artistName, artistNameNew);
}

bool ArtistLibrary::addMusicToPlayList(const QString &artistName, const QString &playListName) const {
    return SQLite::getInstance().artistRepository.addMusicToPlayList(artistName, playListName);
}

ArtistLibraryModel *ArtistLibrary::model() {
    return m_model;
}

DataLoader *ArtistLibrary::loader() {
    return m_loader;
}

void ArtistLibrary::loadByKey(const QString &key) {
    m_model->loadByKey(key, m_loader->getLoadSize(), 0);
}

void ArtistLibrary::loadMoreByKey(int index, const QString &key) {
    m_model->loadMoreByKey(key, m_loader->getLoadSize(), index);
}
