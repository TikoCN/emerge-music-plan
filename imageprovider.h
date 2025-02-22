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

    //加载网络类型
    QPixmap downOnlineCover(int id);

    //加载本地封面
    QPixmap loadFileCover(int id);
};

#endif // IMAGEPROVIDER_H
