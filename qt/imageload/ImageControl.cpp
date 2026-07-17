#include "ImageControl.h"

ImageControl::ImageControl()
= default;

ImageControl::~ImageControl()
= default;

void ImageControl::writeImgCache(const QString &url, const QImage &img) {
    QMutexLocker locker(&m_mutex);

    CacheCell cell(img);
    m_cache.put(url, std::move(cell));
}

QImage ImageControl::getImgCache(const QString &url) {
    QMutexLocker locker(&m_mutex);

    const auto ptr = m_cache.get(url);
    if (!ptr) { return {}; }
    return ptr->img;
}

void ImageControl::writeUrlNullFlag(const QString &url, const bool flag) {
    QMutexLocker locker(&m_mutex);

    const auto it = m_cache.get(url);
    if (it)
        it->isNUll = flag;
}

bool ImageControl::getUrlNullFlag(const QString &url) {
    QMutexLocker locker(&m_mutex);

    const auto it = m_cache.get(url);
    if (!it) { return true; }
    return it->isNUll;
}
