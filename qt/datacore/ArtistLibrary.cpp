#include "ArtistLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "ArtistLibraryModel.h"

ArtistLibrary::ArtistLibrary() : m_model(new ArtistLibraryModel(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildArtistPlayer,
            this, &ArtistLibrary::buildArtistPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &ArtistLibrary::finish);
}

ArtistLibrary::~ArtistLibrary() {
    delete m_model;
}

void ArtistLibrary::updateArtistName(const int artistId, const QString &name) {
    DataActive::getInstance().updateArtistName(artistId, name);
}

void ArtistLibrary::updateArtistSort(const int artistId, const int sort) {
    DataActive::getInstance().updateArtistSort(artistId, sort);
}

Artist ArtistLibrary::getArtistData(const int id) {
    return DataActive::getInstance().getArtistData(id);
}

QJsonObject ArtistLibrary::getArtistJson(const int id) {
    return DataActive::getInstance().getArtistJson(id);
}

QStringList ArtistLibrary::getArtistKeys() const {
    return SQLite::getInstance().getArtistKeys();
}

int ArtistLibrary::checkArtistName(const QString &name) const {
    return SQLite::getInstance().checkArtistName(name);
}

bool ArtistLibrary::moveArtistMusic(const QString &artistName, const QString &artistNameNew) const {
    return SQLite::getInstance().moveArtistMusic(artistName, artistNameNew);
}

bool ArtistLibrary::addArtistMusicToPlayList(const QString &artistName, const QString &playListName) const {
    return SQLite::getInstance().addArtistMusicToPlayList(artistName, playListName);
}

ArtistLibraryModel *ArtistLibrary::model() {
    return m_model;
}