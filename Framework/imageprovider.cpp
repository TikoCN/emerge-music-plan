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
void ImageResponse::loadMusicFile(int id)
{
    try {
        FFmpeg ffmpeg;
        QString musicUrl = SQLite::getInstance()->getMusicUrl(id);

        //提取附加封面
        if (QFile::exists(musicUrl))
            m_img = ffmpeg.getInlayCover(musicUrl);
        if (!m_img.isNull())  throw (true);

        QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";
        loadImageFile(coverUrl);
        if (m_img.isNull())
            throw QString("加载失败，读取默认封面");

    } catch (QString e) {
        m_img.load(":/image/default.png");
    } catch (bool finish) {
        // 加载完成，提前退出
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadMusicOnline(int id)
{
    try {
        FFmpeg ffmpeg;
        QString musicUrl = SQLite::getInstance()->getMusicUrl(id);

        //提取附加封面
        if (QFile::exists(musicUrl))
            m_img = ffmpeg.getInlayCover(musicUrl);
        if(!m_img.isNull())  throw (true);

        QString coverUrl = Base::getInstance()->getBaseUrl(musicUrl) + ".jpg";
        if (coverUrl == "")
            throw QString("路径错误，读取默认封面");

        OnLine::getInstance()->downMusicCover(Base::getInstance()->getFileName(musicUrl), coverUrl);
        loadImageFile(coverUrl);

        if (m_img.isNull())
            throw QString("下载失败，读取默认封面");

    } catch (QString e) {
        m_img.load(":/image/default.png");
    } catch (bool finish) {
        // 加载完成，提前退出
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadTableFile(int id)
{
    MusicCore *core = MusicCore::getInstance();
    try {
        if (id < 0)
            throw false;

        Table *table = core->getTable(id);
        if (table == nullptr)
            throw false;

        if (table->musicList.size() > 0)
            loadMusicFile(table->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r) {
            m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
            if (!m_img.isNull()) buildRoundImage(&m_img, 10);
            m_img.load(":/image/default.png");
        }
    }
}

void ImageResponse::loadTableOnline(int id)
{
    MusicCore *core = MusicCore::getInstance();
    try {
        if (id < 0)
            throw false;

        Table *table = core->getTable(id);
        if (table == nullptr)
            throw false;

        if (table->musicList.size() > 0)
            loadMusicOnline(table->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r) {
            m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
            if (!m_img.isNull()) buildRoundImage(&m_img, 10);
            m_img.load(":/image/default.png");
        }
    }
}

void ImageResponse::loadArtistFile(int id)
{
    MusicCore *core = MusicCore::getInstance();
    Base *base = Base::getInstance();
    try {
        if (id < 0)
            throw false;

        Artist *artist = core->getArtist(id);
        if (artist == nullptr)
            throw false;

        QString coverUrl = base->getArtistCoverUrl(artist->name);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        if (artist->musicList.size() > 0)
            loadMusicFile(artist->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r)
            m_img.load(":/image/default.png");
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadArtistOnline(int id)
{
    MusicCore *core = MusicCore::getInstance();
    Base *base = Base::getInstance();
    OnLine *online = OnLine::getInstance();

    try {
        if (id < 0)
            throw false;

        Artist *artist = core->getArtist(id);
        if (artist == nullptr)
            throw false;

        // 检测是否存在下载文件
        QString coverUrl = base->getArtistCoverUrl(artist->name);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        // 联网下载
        online->downArtistCover(artist->name, coverUrl);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        if (artist->musicList.size() > 0)
            loadMusicFile(artist->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r)
            m_img.load(":/image/default.png");
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadAlbumFile(int id)
{
    MusicCore *core = MusicCore::getInstance();
    Base *base = Base::getInstance();
    try {
        if (id < 0)
            throw false;

        Album *album = core->getAlbum(id);
        if (album == nullptr)
            throw false;

        QString coverUrl = base->getAlbumCoverUrl(album->name);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        if (album->musicList.size() > 0)
            loadMusicFile(album->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r)
            m_img.load(":/image/default.png");
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
}

void ImageResponse::loadAlbumOnline(int id)
{
    MusicCore *core = MusicCore::getInstance();
    Base *base = Base::getInstance();
    OnLine *online = OnLine::getInstance();

    try {
        if (id < 0)
            throw false;

        Album *album = core->getAlbum(id);
        if (album == nullptr)
            throw false;

        // 检测是否存在下载文件
        QString coverUrl = base->getAlbumCoverUrl(album->name);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        // 联网下载
        online->downAlbumCover(album->name, coverUrl);
        loadImageFile(coverUrl);
        if (!m_img.isNull())
            throw true;

        if (album->musicList.size() > 0)
            loadMusicFile(album->musicList.first());
        else
            throw false;

        if (m_img.isNull())
            throw false;

    } catch (bool r) {
        if (!r)
            m_img.load(":/image/default.png");
    }

    m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
    if (!m_img.isNull()) buildRoundImage(&m_img, 10);
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
        "artistFile", "artistOnline",
        "albumFile", "albumOnline",
        "tableFile", "tableOnline"
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
