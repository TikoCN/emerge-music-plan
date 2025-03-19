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

QImage ImageResponse::downOnlineCover(int id)
{
    OnLine *onLine = OnLine::getInstance();
    MusicCore *core = MusicCore::getInstance();
    QImage pix;

    //加载附加封面，和独立封面
    pix = core->musicList[id]->loadCover();

    if(pix.isNull()){
        //从网络下载封面，并报错到独立封面
        onLine->downCover(core->musicList[id]->getSearchString(), core->musicList[id]->getCoverUrl());
    }

    //加载附加封面，和独立封面
    pix = core->musicList[id]->loadCover();
    return pix;
}

QImage ImageResponse::loadFileCover(int id)
{
    MusicCore *core = MusicCore::getInstance();

    if(core->musicList[id]->cover != NULL){//读取已经加载好的封面
        return *core->musicList[id]->cover;
    }
    return core->musicList[id]->loadCover();
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
    int coreId = url.split(":").last().toInt();
    MusicCore *core = MusicCore::getInstance();

    if(coreId < -1 || coreId > core->musicList.size()){
        img.load(":/image/default.jpg");
    }
    else if(type == "onLine"){//在线加载
        img = downOnlineCover(coreId);
    }
    else if(type == "file"){//本地加载
        img = loadFileCover(coreId);
    }
    else if(type == "back"){
        img = downOnlineCover(coreId);
        emit finished();
        return;
    }

    //找不封面，设置为默认封面
    if(img.isNull()){
        img.load(":/image/default.jpg");
    }

    img = img.scaled(requestedSize);
    buildRoundImage(&img, 10);

    emit finished();
}

QQuickImageResponse *ImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    class ImageResponse *res = new class ImageResponse(id, requestedSize);
    pool.start(res);
    return res;
}
