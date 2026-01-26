#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QThreadPool>
#include <QRunnable>
#include <QQuickImageResponse>
#include "imageload/imagecontrol.h"
#include "datacore/dataactive.h"

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

    ImageResponse(QString url, const QSize &requestedSize);
    ~ImageResponse() override;

    [[nodiscard]] QQuickTextureFactory *textureFactory() const override;

    void run() override;

    //计算圆角图片
    void buildRoundImage();
    bool loadImageFile(const QString& url);

    void loadMusicCover(bool isOnline);
    void loadPlayListCover(bool isOnline);
    void loadArtistCover(bool isOnline);
    void loadAlbumCover(bool isOnline);

    static ImageType typeFromStringToEnum(const QString& type);

    [[nodiscard]] QString errorString() const override;

private:
    QImage m_img;
    QString m_url;
    QString m_loadType;
    QString m_errorString;
    QSize m_requestedSize;

    ImageControl *ctr;
    DataActive *data;

    int m_loadId{};
    int m_loadMusicId{};
    int m_radius{};

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
