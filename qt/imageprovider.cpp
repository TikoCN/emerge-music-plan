#include "imageprovider.h"
#include <QPixmapCache>
#include "online.h"
#include <QPainter>
#include <QPainterPath>
#include <QThread>
#include <QImageReader>
#include "sqlite/sqlite.h"
#include "ffmpeg.h"
#include "base.h"
#include "musiccore.h"


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
void ImageResponse::loadMusicFile(int id, int radius)
{
    FFmpeg ffmpeg;
    QString musicUrl = SQLite::getInstance()->getMusicUrl(id);

    //提取附加封面
    if (QFile::exists(musicUrl))
        m_img = ffmpeg.getInlayCover(musicUrl);

    if (m_img.isNull()) {
        QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";
        loadImageFile(coverUrl);
    }

    if (m_img.isNull()) {
        MusicCore::getInstance()->setMusicIsNoCover(id, false);
        return;
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, radius);
    MusicCore::getInstance()->setMusicIsNoCover(id, false);
}

void ImageResponse::loadMusicOnline(int id, int radius)
{
    FFmpeg ffmpeg;
    QString musicUrl = SQLite::getInstance()->getMusicUrl(id);

    //提取附加封面
    if (QFile::exists(musicUrl))
        m_img = ffmpeg.getInlayCover(musicUrl);

    QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";
    if (m_img.isNull())
        loadImageFile(coverUrl);

    if (m_img.isNull()) {
        if (coverUrl == "")
            OnLine::getInstance()->downMusicCover(Base::getInstance()->getFileName(musicUrl), coverUrl);
        loadImageFile(coverUrl);
    }

    if (m_img.isNull())
        return;

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&m_img, radius);
}

void ImageResponse::loadTableFile(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    Table *table = nullptr;
    try {
        if (id < 0)
            throw false;

        table = core->getTable(id);
        if (table == nullptr)
            throw false;

        if (table->musicList.size() > 0)
            loadMusicFile(table->musicList.first(), radius);
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r) {
            m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
            if (!m_img.isNull()) buildRoundImage(&m_img, radius);
            m_img.load(":/image/default.png");
        }
    }

    if (table == nullptr) core->releaseTable(id);
}

void ImageResponse::loadTableOnline(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    Table *table = nullptr;
    try {
        if (id < 0)
            throw false;

        table = core->getTable(id);
        if (table == nullptr)
            throw false;

        if (table->musicList.size() > 0)
            loadMusicOnline(table->musicList.first(), radius);
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r) {
            m_img.load(":/image/default.png");
            m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
            if (!m_img.isNull()) buildRoundImage(&m_img, radius);
        }
    }

    if (table == nullptr) core->releaseTable(id);
}

void ImageResponse::loadArtistFile(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    QString name = core->getArtistName(id);
    QString url = Base::getInstance()->getArtistCoverUrl(name);
    loadImageFile(url);

    if(m_img.isNull()){
        core->setArtistIsNoCover(id, false);
        return;
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&m_img, radius);
    core->setArtistIsNoCover(id, true);
}

void ImageResponse::loadArtistOnline(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    QString name = core->getArtistName(id);
    QString url = Base::getInstance()->getArtistCoverUrl(name);
    loadImageFile(url);

    if(m_img.isNull()){
        OnLine::getInstance()->downArtistCover(name, url);
        loadImageFile(url);
    }

    if(m_img.isNull()){
        core->setArtistIsNoCover(id, false);
        return;
    }
    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&m_img, radius);
    core->setArtistIsNoCover(id, true);
}

void ImageResponse::loadAlbumFile(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    QString name = core->getAlbumName(id);
    QString url = Base::getInstance()->getAlbumCoverUrl(name);
    loadImageFile(url);

    if(m_img.isNull()){
        core->setAlbumIsNoCover(id, true);
        return;
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&m_img, radius);
    core->setAlbumIsNoCover(id, true);
}

void ImageResponse::loadAlbumOnline(int id, int radius)
{
    MusicCore *core = MusicCore::getInstance();
    QString name = core->getAlbumName(id);
    QString url = Base::getInstance()->getAlbumCoverUrl(name);
    loadImageFile(url);

    if(m_img.isNull()){
        OnLine::getInstance()->downAlbumCover(name, url);
        loadImageFile(url);
    }

    if(m_img.isNull()){
        core->setAlbumIsNoCover(id, true);
        return;
    }
    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    buildRoundImage(&m_img, radius);
    core->setAlbumIsNoCover(id, true);
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
    QRegularExpression rx;
    rx.setPattern(R"(([^?]+)\?)");
    QString type = "";
    QRegularExpressionMatch r = rx.match(m_url);
    if (r.hasMatch())
        type = r.capturedTexts().at(1);

    int id = 0;
    rx.setPattern(R"(id=(\d+))");
    r = rx.match(m_url);
    if (r.hasMatch())
        id = r.capturedTexts().at(1).toInt();

    int radius = 10;
    rx.setPattern(R"(raidus=(\d+))");
    r = rx.match(m_url);
    if (r.hasMatch())
        radius = r.capturedTexts().at(1).toInt();

    QStringList typeList = {
        "musicFile", "musicOnLine",
        "artistFile", "artistOnline",
        "albumFile", "albumOnline",
        "tableFile", "tableOnline"
    };
    switch (typeList.indexOf(type)) {
    case 0:
        loadMusicFile(id, radius);
        break;
    case 1:
        loadMusicOnline(id, radius);
        break;
    case 2:
        loadArtistFile(id, radius);
        break;
    case 3:
        loadArtistOnline(id, radius);
        break;
    case 4:
        loadAlbumFile(id, radius);
        break;
    case 5:
        loadAlbumOnline(id, radius);
        break;
    case 6:
        loadTableFile(id, radius);
        break;
    case 7:
        loadTableOnline(id, radius);
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
