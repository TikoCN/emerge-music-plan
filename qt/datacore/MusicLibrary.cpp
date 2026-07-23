#include "MusicLibrary.h"
#include "DataActive.h"
#include "sqlite/Sqlite.h"
#include "MusicLibraryModel.h"

MusicLibrary::MusicLibrary() : m_model(new MusicLibraryModel(this)), m_loader(new DataLoader(this)) {
    connect(m_loader, &DataLoader::loadInitData, this, &MusicLibrary::loadByKey);
    connect(m_loader, &DataLoader::loadData, this, &MusicLibrary::loadMoreByKey);
}

MusicLibrary::~MusicLibrary() {
    delete m_model;
    delete m_loader;
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