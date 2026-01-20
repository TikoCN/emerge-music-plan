#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QThreadPool>
#include <QRunnable>
#include <QObject>
#include <QQuickAsyncImageProvider>
#include <QQuickImageResponse>
#include "imageload/imagecontrol.h"
#include "musiccore.h"

class ImageResponse : public QQuickImageResponse, public QRunnable{
    Q_OBJECT

public:

    // 默认参数

    enum ImageType {
        MusicFile,
        MusicOnline,
        ArtistFile,
        ArtistOnline,
        AlbumFile,
        AlbumOnline,
        PlayListFile,
        PlayListOnline,
        Unknown
    };

    ImageResponse(const QString &url, const QSize &requestedSize);
    ~ImageResponse();

    QQuickTextureFactory *textureFactory() const override;

    void run() override;

    //计算圆角图片
    void buildRoundImage();
    bool loadImageFile(QString url);

<<<<<<<< Updated upstream:imageprovider.h
    void loadMusicFile(int id);
    void loadMusicOnline(int id);
    void loadTableFile(int id);
    void loadTableOnline(int id);
    void loadArtistFile(int id);
    void loadArtistOnline(int id);
    void loadAlbumFile(int id);
    void loadAlbumOnline(int id);
========
    void loadMusicCover(bool isOnline);
    void loadPlayListCover(bool isOnline);
    void loadArtistCover(bool isOnline);
    void loadAlbumCover(bool isOnline);

    ImageType typeFromStringToEnum(QString type);

    QString errorString() const override;
>>>>>>>> Stashed changes:imageload/imageprovider.h

private:
    QImage m_img;
    QString m_url;
    QString m_loadType;
    QString m_errorString;
    QSize m_requestedSize;

    ImageControl *ctr;
    MusicCore *core;

    int m_loadId;
    int m_loadMusicId;
    int m_radius;

};

class ImageProvider : public QQuickAsyncImageProvider{
    Q_OBJECT

public:
    QQuickImageResponse *requestImageResponse(const QString &id, const QSize &requestedSize) override;

private:
    // 加载图片的线程池
    QThreadPool pool;
};


#endif // IMAGEPROVIDER_H
