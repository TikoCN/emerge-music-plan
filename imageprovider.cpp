#include "imageprovider.h"
#include "musiccore.h"
#include <QPixmapCache>
#include "online.h"
#include <QPainter>
#include <QPainterPath>
#include <QThread>
#include <QImageReader>
#include "sqlite/sqlite.h"
#include "ffmpeg.h"
#include "base.h"


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
    QString url = SQLite::getInstance()->getMusicUrl(id);
    if (url == "") return;

    OnLine *onLine = OnLine::getInstance();
    FFmpeg ffmpeg;
    QString coverUrl = Base::getInstance()->getBaseUrl(url) + ".jpg";
    //提取附加封面
    m_img = ffmpeg.getInlayCover(url);

    // 检测独立封面
    if(m_img.isNull() && QFile::exists(coverUrl)) loadFileCover(coverUrl);

    // 下载网络封面
    if(m_img.isNull()) onLine->downCover(Base::getInstance()->getFileName(url), coverUrl);

    // 检测独立封面
    if(m_img.isNull() && QFile::exists(coverUrl)) loadFileCover(coverUrl);

    if (m_img.isNull()) m_img.load(":/image/default.png");
    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);

    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadFileCover(int id)
{
    OnLine *onLine = OnLine::getInstance();
    QString url = SQLite::getInstance()->getMusicUrl(id);
    if (url == "") return;
    FFmpeg ffmpeg;
    QString coverUrl = Base::getInstance()->getBaseUrl(url) + ".jpg";

    //提取附加封面
    m_img = ffmpeg.getInlayCover(url);

    if(m_img.isNull() && QFile::exists(coverUrl)) loadFileCover(coverUrl);

    if (m_img.isNull()) m_img.load(":/image/default.png");
    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);

    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadFileCover(QString url)
{
    //如果存在 独立封面
    if(QFile::exists(url))
    {
        QImageReader reader;
        reader.setFileName(url);
        QSize aim = reader.size();
        aim.scale(QSize(300,300), Qt::KeepAspectRatioByExpanding);
        reader.setScaledSize(aim);
        m_img = reader.read();
    }
}

ImageResponse::ImageResponse(const QString &url, const QSize &requestedSize)
    :m_url(url), m_requestedSize(requestedSize)
{
    setAutoDelete(false);
}

QQuickTextureFactory *ImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_img);
}

void ImageResponse::run()
{
    QString type = m_url.split(":").first();
    int id = m_url.split(":").last().toInt();

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

    emit finished();
}

QQuickImageResponse *ImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    class ImageResponse *res = new class ImageResponse(id, requestedSize);
    pool.start(res);
    return res;
}
