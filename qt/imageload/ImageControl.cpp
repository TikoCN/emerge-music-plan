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

/**
 * @brief 记录请求 URL 的加载结果，与按采样尺寸保存的图片缓存分开。
 * @param url 界面使用的完整请求 URL。
 * @param flag 图片是否加载失败。
 */
void ImageControl::writeUrlNullFlag(const QString &url, const bool flag) {
    QMutexLocker locker(&m_mutex);

    const auto it = m_cache.get(url);
    if (it) {
        it->isNUll = flag;
    } else {
        CacheCell cell{QImage()};
        cell.isNUll = flag;
        m_cache.put(url, std::move(cell));
    }
}

bool ImageControl::getUrlNullFlag(const QString &url) {
    QMutexLocker locker(&m_mutex);

    const auto it = m_cache.get(url);
    if (!it) { return true; }
    return it->isNUll;
}
