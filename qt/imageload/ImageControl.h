#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include <QMutex>
#include "baseclass/LruCache.h"

class ImageControl : public QObject {
    Q_OBJECT

private:
    struct CacheCell {
        bool   isNUll;
        QImage img;
        QSize  size;

        explicit CacheCell(const QImage &img)
            : isNUll(false),
              img(img) {
        }
    };

    // 缓存
    const int                    MAX_CACHE_SIZE = 50;
    LruCache<QString, CacheCell> m_cache{MAX_CACHE_SIZE};
    QMutex                       m_mutex;

    explicit ImageControl();
    ~ImageControl() override;

public:
    static ImageControl &getInstance() {
        static ImageControl instance;
        return instance;
    }

    void   writeImgCache(const QString &url, const QImage &img);
    QImage getImgCache(const QString &url);

    void             writeUrlNullFlag(const QString &url, bool flag);
    Q_INVOKABLE bool getUrlNullFlag(const QString &url);
};

#endif // IMAGECONTROL_H
