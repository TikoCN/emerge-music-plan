#ifndef IMAGECONTROL_H
#define IMAGECONTROL_H

#include <QObject>
#include <QHash>
#include <QImage>
#include <QDebug>
#include <QMutex>

class ImageControl : public QObject
{
    Q_OBJECT
private:
    static ImageControl* instance;
    QHash<QString, bool> m_urlNullHash;
    QStringList m_urlNullList;
    QMutex m_urlNullMutex;

    QHash<QString, QImage> m_imgCacheHash;
    QStringList m_imgCacheList;
    QMutex m_imgCacheMutex;

    const int MAX_HASH_SIZE = 50;

    explicit ImageControl();
    ~ImageControl();
public:

    static ImageControl* getInstance(){
        return instance;
    }

    static void buildInstance(){
        if(instance == nullptr){
            instance = new ImageControl;
        }
    }

    static void freeInstance(){
        if(instance != nullptr){
            delete instance;
        }
    }

    Q_INVOKABLE const bool getUrlNullFlag(QString url);

    const QImage getImgCache(QString url);

    void writeImgCache(QString url, const QImage img);

    void writeUrlNullFlag(QString url, const bool flag);
};

#endif // IMAGECONTROL_H
