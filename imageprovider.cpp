#include "imageprovider.h"
#include <QPixmapCache>
#include "mediaplayer.h"
#include <QPainter>
#include <QPainterPath>

ImageProvider::ImageProvider()
: QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

/*
 *重写requestPixmap函数
 */
QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    MediaPlayer *player = MediaPlayer::getInstance();
    QPixmap pix;
    QString type = id.split(":").first();
    int coreId = id.split(":").last().toInt();

    if(type == "onLine"){
        pix = player->coreList[coreId]->loadCoverOnline();
    }
    else if(type == "file"){
        if(player->coreList[coreId]->cover != NULL){
            return *player->coreList[coreId]->cover;
        }
        pix = player->coreList[coreId]->loadCover();
    }

    pix = pix.scaled(requestedSize);
    buildRoundImage(&pix, requestedSize.width() * 0.2);

    if(type == "file"){
        player->coreList[coreId]->cover = new QPixmap;
        *player->coreList[coreId]->cover = pix;
    }
    return pix;
}

/*
 * 计算圆角图片
 */
void ImageProvider::buildRoundImage(QPixmap *pix, int radius)
{
    QRect rect = pix->rect();//得到大小
    QPixmap destImage(rect.size());//目标结果
    destImage.fill(Qt::transparent);

    QPainter painter(&destImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    // 创建一个QPainterPath用于裁剪圆角矩形
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);

    painter.drawPixmap(rect, *pix);

    *pix = destImage;
}
