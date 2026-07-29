#include "ArtistLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/ArtistModel.h"

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
