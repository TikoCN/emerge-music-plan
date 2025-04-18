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

    // 加载网络封面
    void downOnlineCover(int musicId);

    // 加载本地封面
    void loadFileCover(int musicId);

    void loadFileCover(QString url);

    // 加载本地封面，裁剪版
    void loadFileClipCover(int musicId);

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
