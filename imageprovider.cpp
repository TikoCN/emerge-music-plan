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
void ImageResponse::loadMusicFile(int id)
{
    try {
        FFmpeg ffmpeg;
        QString musicUrl = SQLite::getInstance()->getMusicUrl(id);
        if (musicUrl == "" || !QFile::exists(musicUrl)) {
            throw QString("路径错误，读取默认封面");
        }
        QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";

        //提取附加封面
        m_img = ffmpeg.getInlayCover(musicUrl);

        if(m_img.isNull() && QFile::exists(coverUrl)) {
            loadImageFile(coverUrl);
        }
    } catch (QString e) {
        if (m_img.isNull()) m_img.load(":/image/default.png");
        m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    }

    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadMusicOnline(int id)
{
    try {
        FFmpeg ffmpeg;
        QString musicUrl = SQLite::getInstance()->getMusicUrl(id);
        if (musicUrl == "" || !QFile::exists(musicUrl)) {
            throw QString("路径错误，读取默认封面");
        }
        QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";

        //提取附加封面
        m_img = ffmpeg.getInlayCover(musicUrl);

        // 下载网络封面
        if(m_img.isNull()) {
            OnLine::getInstance()->downCover(Base::getInstance()->getFileName(musicUrl), coverUrl);
        }

        if(m_img.isNull() && QFile::exists(coverUrl)) {
            loadImageFile(coverUrl);
        }
    } catch (QString e) {
        if (m_img.isNull()) m_img.load(":/image/default.png");
        m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    }

    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadTableFile(int id)
{

}

void ImageResponse::loadTableOnline(int id)
{

}

void ImageResponse::loadArtistFile(int id)
{

}

void ImageResponse::loadArtistOnline(int id)
{

}

void ImageResponse::loadAlbumFile(int id)
{

}

void ImageResponse::loadAlbumOnline(int id)
{

}

void ImageResponse::loadImageFile(QString url)
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

    QStringList typeList = {
        "musicFile", "musicOnLine",
        "artistFile", "artistOnLine",
        "albumFile", "albumOnLine",
        "tableFile", "tableOnLine"
    };
    switch (typeList.indexOf(type)) {
    case 0:
        loadMusicFile(id);
        break;
    case 1:
        loadMusicOnline(id);
        break;
    case 2:
        loadArtistFile(id);
        break;
    case 3:
        loadArtistOnline(id);
        break;
    case 4:
        loadAlbumFile(id);
        break;
    case 5:
        loadAlbumOnline(id);
        break;
    case 6:
        loadTableFile(id);
        break;
    case 7:
        loadTableOnline(id);
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
