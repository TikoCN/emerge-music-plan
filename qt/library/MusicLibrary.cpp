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

Music MusicLibrary::getData(const int id) {
    return DataActive::getInstance().getMusicData(id);
}

QStringList MusicLibrary::getKeys() {
    return SQLite::getInstance().musicRepository.getKeys();
}
