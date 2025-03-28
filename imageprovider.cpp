#include "imageprovider.h"
#include "musiccore.h"
#include <QPixmapCache>
#include "online.h"
#include <QPainter>
#include <QPainterPath>
#include <QThread>


/*
 * 计算圆角图片
 */
void ImageResponse::buildRoundImage(QImage *pix, int radius)
{
    QRect rect = pix->rect();//得到大小
    QImage destImage(rect.width(), rect.height(), QImage::Format_ARGB32);//目标结果
    destImage.fill(Qt::transparent);

    QPainter painter(&destImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    // 创建一个QPainterPath用于裁剪圆角矩形
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);
    painter.setClipPath(path);

    painter.drawImage(rect, *pix);

    *pix = destImage;
}

void ImageResponse::downOnlineCover(int id)
{
    OnLine *onLine = OnLine::getInstance();
    MusicCore *core = MusicCore::getInstance();
    if(id < 0 || id >core->musicList.size()){
        return;
    }

    //加载附加封面，和独立封面
    img = core->musicList[id]->loadCover();

    if(img.isNull()){
        //从网络下载封面，并写入到独立文件
        onLine->downCover(core->musicList[id]->getSearchString(), core->musicList[id]->getCoverUrl());
    }

    // 检测加载附加封面，和独立封面
    img = core->musicList[id]->loadCover();
    img = img.scaled(requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&img, 10);
}

void ImageResponse::loadFileCover(int id)
{
    MusicCore *core = MusicCore::getInstance();
    if(id < 0 || id >core->musicList.size()){
        return;
    }
    img = core->musicList[id]->loadCover();
    img = img.scaled(requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&img, 10);
}

void ImageResponse::loadFileClipCover(int id)
{
    MusicCore *core = MusicCore::getInstance();
    if(id < 0 || id >core->musicList.size()){
        return;
    }
    img = core->musicList[id]->loadCover();
    img = img.scaled(requestedSize, Qt::KeepAspectRatioByExpanding);
}

ImageResponse::ImageResponse(const QString &url, const QSize &requestedSize)
{
    setAutoDelete(false);
    this->url = url;
    this->requestedSize = requestedSize;
}

QQuickTextureFactory *ImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(img);
}

void ImageResponse::run()
{
    QString type = url.split(":").first();
    int id = url.split(":").last().toInt();

    QStringList typeList = {"file", "fileClip", "back", "onLine"};
    switch (typeList.indexOf(type)) {
    case 0:
        loadFileCover(id);
        break;
    case 1:
        loadFileClipCover(id);
        break;
    case 2:
        downOnlineCover(id);
        break;
    case 3:
        downOnlineCover(id);
        break;
    default:
        break;
    }

    //找不封面，设置为默认封面
    if(img.isNull()){
        img.load(":/image/default.png");
    }

    emit finished();
}

QQuickImageResponse *ImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    class ImageResponse *res = new class ImageResponse(id, requestedSize);
    pool.start(res);
    return res;
}
