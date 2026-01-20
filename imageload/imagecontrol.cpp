#include "imagecontrol.h"

ImageControl::ImageControl()
{
}

ImageControl::~ImageControl()
{
}

const bool ImageControl::getUrlNullFlag(QString url)
{
    QMutexLocker locker(&m_urlNullMutex);

    if(url.contains("image://cover/"))
        url = url.split("image://cover/").last();
    else
        return false;


    if (m_urlNullHash.contains(url)) {
        return m_urlNullHash.value(url);
    }

    int pos = m_urlNullList.indexOf(url);
    if (pos != -1) {
        m_urlNullList.move(pos, m_urlNullList.size() - 1);
    }

    return false;
}

void ImageControl::writeUrlNullFlag(QString url, bool flag)
{
    QMutexLocker locker(&m_urlNullMutex);

    m_urlNullHash.insert(url, flag);

    int pos = m_urlNullList.indexOf(url);
    if (pos != -1) {
        m_urlNullList.move(pos, m_urlNullList.size() - 1);
    } else {
        m_urlNullList.append(url);
    }

    if (m_urlNullList.size() > MAX_HASH_SIZE) {
        QString deleteUrl = m_urlNullList.takeFirst();
        m_urlNullHash.remove(deleteUrl);
    }
}

const QImage ImageControl::getImgCache(QString url)
{
    QMutexLocker locker(&m_imgCacheMutex);

    if (m_imgCacheHash.contains(url)) {
        return m_imgCacheHash.value(url);
    }

    int pos = m_imgCacheList.indexOf(url);
    if (pos != -1) {
        m_imgCacheList.move(pos, m_imgCacheList.size() - 1);
    }

    return QImage();
}

void ImageControl::writeImgCache(QString url, QImage img)
{
    QMutexLocker locker(&m_imgCacheMutex);
    m_imgCacheHash.insert(url, img);

    int pos = m_imgCacheList.indexOf(url);
    if (pos != -1) {
        m_imgCacheList.move(pos, m_imgCacheList.size() - 1);
    } else {
        m_imgCacheList.append(url);
    }

    if (m_imgCacheList.size() > MAX_HASH_SIZE) {
        QString deleteUrl = m_imgCacheList.takeFirst();
        m_imgCacheHash.remove(deleteUrl);
    }
}
