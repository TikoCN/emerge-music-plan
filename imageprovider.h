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

    //加载网络类型
    QImage downOnlineCover(int id);

    //加载本地封面
    QImage loadFileCover(int id);

private:
    QImage img;
    QString url;
    QSize requestedSize;
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
