#include "MusicLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/MusicModel.h"

QList<int> MusicLibrary::listSort(const QList<int> &musicIdList, const SORT_TYPE sort) {
    return DataActive::getInstance().musicListSort(musicIdList, sort);
}

void MusicLibrary::updateLove(const int musicId, const bool isLove) {
    DataActive::getInstance().updateMusicLove(musicId, isLove);
}

void MusicLibrary::updateLevel(const int musicId, const bool level) {
    DataActive::getInstance().updateMusicLevel(musicId, level);
}

Music MusicLibrary::getMusicData(const int id) {
    return DataActive::getInstance().getMusicData(id);
}

QJsonObject MusicLibrary::getJson(const int id) {
    return DataActive::getInstance().getMusicJson(id);
}

QStringList MusicLibrary::getKeys() const {
    return SQLite::getInstance().musicRepository.getKeys();
}

void MusicLibrary::startClearInvalidData() {
    SQLite::getInstance().startClearInvalidData();
}
