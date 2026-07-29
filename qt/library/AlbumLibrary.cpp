#include "AlbumLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"

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
