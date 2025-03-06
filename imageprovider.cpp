#include "imageprovider.h"
#include <QPixmapCache>
#include "mediaplayer.h"
#include "online.h"
#include <QPainter>
#include <QPainterPath>
#include <QThread>

ImageProvider::ImageProvider()
: QQuickImageProvider(QQuickImageProvider::Pixmap)
{
    QThread *thread = new QThread;
    this->moveToThread(thread);
    thread->start();
}

/*
 *重写requestPixmap函数
 */
QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QPixmap pix;
    QString type = id.split(":").first();
    int coreId = id.split(":").last().toInt();
    MediaPlayer *player = MediaPlayer::getInstance();
    if(coreId < -1 || coreId > player->coreList.size()){
        pix.load(":/image/default.jpg");
    }
    else if(type == "onLine"){//在线加载
        pix = downOnlineCover(coreId);
    }
    else if(type == "file"){//本地加载
        pix = loadFileCover(coreId);
    }
    else if(type == "back"){
        pix = downOnlineCover(coreId);
        return pix;
    }

    //找不封面，设置为默认封面
    if(pix.isNull()){
        pix.load(":/image/default.jpg");
    }

    pix = pix.scaled(requestedSize);
    buildRoundImage(&pix, 10);

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

QPixmap ImageProvider::downOnlineCover(int id)
{
    OnLine *onLine = OnLine::getInstance();
    MediaPlayer *player = MediaPlayer::getInstance();
    QPixmap pix;

    //加载附加封面，和独立封面
    pix = player->coreList[id]->loadCover();

    if(pix.isNull()){
        //从网络下载封面，并报错到独立封面
        onLine->downCover(player->coreList[id]->getSearchString(), player->coreList[id]->getCoverUrl());
    }

    //加载附加封面，和独立封面
    pix = player->coreList[id]->loadCover();
    return pix;
}

QPixmap ImageProvider::loadFileCover(int id)
{
    MediaPlayer *player = MediaPlayer::getInstance();

    if(player->coreList[id]->cover != NULL){//读取已经加载好的封面
        return *player->coreList[id]->cover;
    }
    return player->coreList[id]->loadCover();
}

