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

    void loadMusicFile(int id);
    void loadMusicOnline(int id);
    void loadTableFile(int id);
    void loadTableOnline(int id);
    void loadArtistFile(int id);
    void loadArtistOnline(int id);
    void loadAlbumFile(int id);
    void loadAlbumOnline(int id);

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
