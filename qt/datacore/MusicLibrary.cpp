#include "MusicLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "MusicLibraryModel.h"

MusicLibrary::MusicLibrary() : m_model(new MusicLibraryModel(this)) {
}

MusicLibrary::~MusicLibrary() {
    delete m_model;
}

QList<int> MusicLibrary::musicListSort(const QList<int> &musicIdList, const SORT_TYPE sort) {
    return DataActive::getInstance().musicListSort(musicIdList, sort);
}

void MusicLibrary::updateMusicLove(const int musicId, const bool isLove) {
    DataActive::getInstance().updateMusicLove(musicId, isLove);
}

void MusicLibrary::updateMusicLevel(const int musicId, const bool level) {
    DataActive::getInstance().updateMusicLevel(musicId, level);
}

Music MusicLibrary::getMusicData(const int id) {
    return DataActive::getInstance().getMusicData(id);
}

QJsonObject MusicLibrary::getMusicJson(const int id) {
    return DataActive::getInstance().getMusicJson(id);
}

QStringList MusicLibrary::getMusicKeys() const {
    return SQLite::getInstance().getMusicKeys();
}

void MusicLibrary::startClearInvalidData() {
    SQLite::getInstance().startClearInvalidData();
}

MusicLibraryModel *MusicLibrary::model() {
    return m_model;
}