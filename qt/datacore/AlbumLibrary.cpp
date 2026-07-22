#include "AlbumLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "AlbumLibraryModel.h"

AlbumLibrary::AlbumLibrary() : m_model(new AlbumLibraryModel(this)) {
    connect(&DataActive::getInstance(), &DataActive::buildAlbumPlayer,
            this, &AlbumLibrary::buildAlbumPlayer);
    connect(&DataActive::getInstance(), &DataActive::finish,
            this, &AlbumLibrary::finish);
}

AlbumLibrary::~AlbumLibrary() {
    delete m_model;
}

void AlbumLibrary::updateAlbumName(const int albumId, const QString &name) {
    DataActive::getInstance().updateAlbumName(albumId, name);
}

void AlbumLibrary::updateAlbumSort(const int albumId, const int sort) {
    DataActive::getInstance().updateAlbumSort(albumId, sort);
}

Album AlbumLibrary::getAlbumData(const int id) {
    return DataActive::getInstance().getAlbumData(id);
}

QJsonObject AlbumLibrary::getAlbumJson(const int id) {
    return DataActive::getInstance().getAlbumJson(id);
}

QStringList AlbumLibrary::getAlbumKeys() const {
    return SQLite::getInstance().getAlbumKeys();
}

int AlbumLibrary::checkAlbumName(const QString &name) const {
    return SQLite::getInstance().checkAlbumName(name);
}

bool AlbumLibrary::moveAlbumMusic(const QString &albumName, const QString &albumNameNew) const {
    return SQLite::getInstance().moveAlbumMusic(albumName, albumNameNew);
}

bool AlbumLibrary::addAlbumMusicToPlayList(const QString &albumName, const QString &playListName) const {
    return SQLite::getInstance().addAlbumMusicToPlayList(albumName, playListName);
}

AlbumLibraryModel *AlbumLibrary::model() {
    return m_model;
}