#include "MusicLibrary.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "model/MusicLibraryModel.h"

MusicLibrary::MusicLibrary() : m_model(new MusicLibraryModel(this)), m_loader(new DataLoader(this)) {
    connect(m_loader, &DataLoader::loadInitData, this, &MusicLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &MusicLibrary::loadMoreByKey);
}

MusicLibrary::~MusicLibrary() {
    delete m_model;
    delete m_loader;
}

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

MusicLibraryModel *MusicLibrary::model() {
    return m_model;
}

DataLoader *MusicLibrary::loader() {
    return m_loader;
}

void MusicLibrary::loadByKey(const QString &key) {
    m_model->loadByKey(key, m_loader->getLoadSize(), 0);
    m_loader->finishLoading();
}

void MusicLibrary::loadMoreByKey(int index, const QString &key) {
    m_model->loadMoreByKey(key, m_loader->getLoadSize(), index);
    m_loader->finishLoading();
}