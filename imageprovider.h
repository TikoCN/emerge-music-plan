#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageProvider();

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);  //重写requestPixmap函数

    //计算圆角图片
    void buildRoundImage(QPixmap *pix, int radius);
};

#endif // IMAGEPROVIDER_H
