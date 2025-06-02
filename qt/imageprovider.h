#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QThreadPool>
#include <QRunnable>
#include <QObject>
#include <QQuickAsyncImageProvider>
#include <QQuickImageResponse>

class ImageResponse : public QQuickImageResponse, public QRunnable{
    Q_OBJECT

public:
    ImageResponse(const QString &url, const QSize &requestedSize);

    QQuickTextureFactory *textureFactory() const override;

    void run() override;

    //计算圆角图片
    void buildRoundImage(QImage *pix, int radius);
    void loadImageFile(QString url);

    void loadMusicFile(int id, int radius);
    void loadMusicOnline(int id, int radius);
    void loadTableFile(int id, int radius);
    void loadTableOnline(int id, int radius);
    void loadArtistFile(int id, int radius);
    void loadArtistOnline(int id, int radius);
    void loadAlbumFile(int id, int radius);
    void loadAlbumOnline(int id, int radius);

private:
    QImage m_img;
    QString m_url;
    QSize m_requestedSize;
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
