#include "DataLoader.h"

DataLoader::DataLoader(QObject *parent) : QObject(parent) {}

void DataLoader::setLoadSize(int size) {
    m_loadSize = size;
}

int DataLoader::getLoadSize() const {
    return m_loadSize;
}

void DataLoader::setCurrentKey(const QString &key) {
    m_currentKey = key;
}

QString DataLoader::getCurrentKey() const {
    return m_currentKey;
}

bool DataLoader::isLoading() const {
    return m_loading.load();
}

bool DataLoader::isLoadInit() const {
    return m_loadInit.load();
}

bool DataLoader::isLoadFinish() const {
    return m_loadIsFinish.load();
}

bool DataLoader::isLoadEnable() const {
    return m_loadEnable.load();
}

void DataLoader::loadMore() {
    if (!m_loadEnable.load() || m_loadIsFinish.load() || !m_loadInit.load() || m_loading.exchange(true)) {
        return;
    }

    emit loadData(m_loadPos.load(), m_currentKey, this);
    m_loadPos.fetch_add(m_loadSize);
}

void DataLoader::reset() {
    m_loadPos.store(m_loadSize);  // 初始加载使用 start=0，所以下次 loadMore 从 loadSize 开始
    m_loadInit.store(false);
    m_loadIsFinish.store(false);
    m_loadEnable.store(true);
    m_loading.store(false);
    init();
}

void DataLoader::init() {
    emit loadInitData(m_currentKey, this);
    m_loadInit.store(true);
}

void DataLoader::setLoadFinish(bool finish) {
    m_loadIsFinish.store(finish);
}

void DataLoader::setLoadEnable(bool enable) {
    m_loadEnable.store(enable);
}

void DataLoader::finishLoading() {
    m_loading.store(false);
}