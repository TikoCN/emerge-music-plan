#include "imageprovider.h"
#include <QPixmapCache>
#include "online.h"
#include <QPainter>
#include <QPainterPath>
#include <QImageReader>
#include <utility>
#include "sqlite/sqlite.h"
#include "ffmpeg.h"
#include "basetool/basetool.h"
#include "datacore/dataactive.h"

namespace {
// 匿名命名空间，只在当前文件可见
const QRegularExpression RX_TYPE_PATTERN(R"(([^?]+)\?)");
const QRegularExpression RX_ID_PATTERN(R"(id=(\d+))");
const QRegularExpression RX_RADIUS_PATTERN(R"(radius=(\d+))");
}

/*
 * 计算圆角图片
 */
void ImageResponse::buildRoundImage()
{
    const QRect rect = m_img.rect();//得到大小
    QImage destImage(rect.width(), rect.height(), QImage::Format_ARGB32);//目标结果
    destImage.fill(Qt::transparent);

    QPainter painter(&destImage);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);

    // 创建一个QPainterPath用于裁剪圆角矩形
    QPainterPath path;
    path.addRoundedRect(rect, m_radius, m_radius);
    painter.setClipPath(path);

    painter.drawImage(rect, m_img);

    m_img = destImage;
}

void ImageResponse::loadMusicCover(const bool isOnline)
{
    FFmpeg ffmpeg;
    const QString musicUrl = SQLite::getInstance()->getMusicUrl(m_loadMusicId);
    QString errorId = tr("歌曲ID: ") +
                      QString::number(m_loadMusicId);
    const QString coverUrl = FileManagement::getBaseUrl(musicUrl) +
                       ".jpg";

    //提取附加封面
    if (QFile::exists(musicUrl))
        m_img = ffmpeg.getInlayCover(musicUrl);


    //加载独立封面
    if (m_img.isNull() && !loadImageFile(coverUrl) && isOnline) {
        //独立封面不存在，下载独立封面
        OnLine::downMusicCover(FileManagement::getFileName(musicUrl), coverUrl);
        loadImageFile(coverUrl);
    }
}

void ImageResponse::loadPlayListCover(const bool isOnline)
{
    if (const PlayListPtr playlist = data->getPlayListCore(m_loadId);
        playlist != nullptr && !playlist->musicList.empty()) {
        m_loadMusicId = playlist->musicList[0];
        loadMusicCover(isOnline);
    }
}

void ImageResponse::loadArtistCover(const bool isOnline)
{
    const ArtistPtr artist = data->getArtistCore(m_loadId);
    if (artist.isNull()) {
        return;
    }

    const QString name = artist->name;

    if (const QString url = FileManagement::getArtistCoverUrl(name);
        !loadImageFile(url) && isOnline) {
        TLog::getInstance()->logIgnore(
            tr("歌手") +
            tr("封面加载失败") +
            tr("开始下载封面"));

        OnLine::downArtistCover(name, url);
        loadImageFile(url);
    }
}

void ImageResponse::loadAlbumCover(const bool isOnline)
{
    const AlbumPtr album = data->getAlbumCore(m_loadId);
    if (album.isNull()) {
        return;
    }

    const QString name = album->name;

    if (const QString url = FileManagement::getAlbumCoverUrl(name); !loadImageFile(url) && isOnline){
        TLog::getInstance()->logIgnore(
            tr("专辑") +
            tr("封面加载失败") +
            tr("开始下载封面"));
        OnLine::downAlbumCover(name, url);
        // 重新加载
        loadImageFile(url);
    }
}

QString ImageResponse::errorString() const
{
    return m_errorString;
}

ImageResponse::ImageType ImageResponse::typeFromStringToEnum(const QString& type)
{
    const QStringList typeList = {
        "musicFile", "musicOnLine",
        "artistFile", "artistOnline",
        "albumFile", "albumOnline",
        "playlistFile", "playlistOnline"
    };
    return static_cast<ImageType>(typeList.indexOf(type));
}

bool ImageResponse::loadImageFile(const QString& url)
{
    //加载封面文件
    if(QFile::exists(url))
    {
        QImageReader reader;
        reader.setFileName(url);
        QSize aim = reader.size();
        aim.scale(m_requestedSize, Qt::KeepAspectRatioByExpanding);
        reader.setScaledSize(aim);
        m_img = reader.read();
        return true;
    }

    return false;
}

ImageResponse::ImageResponse(QString url, const QSize &requestedSize)
    :m_url(std::move(url)), m_requestedSize(requestedSize)
{
    setAutoDelete(false);
    ctr = ImageControl::getInstance();
    data = DataActive::getInstance();
}

ImageResponse::~ImageResponse()
{
    m_img = QImage();
}

QQuickTextureFactory *ImageResponse::textureFactory() const
{
    return QQuickTextureFactory::textureFactoryForImage(m_img);
}

void ImageResponse::run()
{
    m_img = ctr->getImgCache(m_url);
    if(!m_img.isNull()) {
        emit finished();
        return;
    }

    QRegularExpressionMatch r = RX_TYPE_PATTERN.match(m_url);
    if (r.hasMatch())
        m_loadType = r.capturedTexts().at(1);

    r = RX_ID_PATTERN.match(m_url);
    if (r.hasMatch())
        m_loadId = r.capturedTexts().at(1).toInt();

    r = RX_RADIUS_PATTERN.match(m_url);
    if (r.hasMatch())
        m_radius = r.capturedTexts().at(1).toInt();


    switch (typeFromStringToEnum(m_loadType)) {
    case ImageType::MusicFile:
        m_loadMusicId = m_loadId;
        loadMusicCover(false);
        break;
    case ImageType::MusicOnline:
        m_loadMusicId = m_loadId;
        loadMusicCover(true);
        break;
    case ImageType::ArtistFile:
        loadArtistCover(false);
        break;
    case ImageType::ArtistOnline:
        loadArtistCover(true);
        break;
    case ImageType::AlbumFile:
        loadAlbumCover(false);
        break;
    case ImageType::AlbumOnline:
        loadAlbumCover(true);
        break;
    case ImageType::PlayListFile:
        loadPlayListCover(false);
        break;
    case ImageType::PlayListOnline:
        loadPlayListCover(true);
        break;
    default:
        break;
    }

    ctr->writeUrlNullFlag(m_url, m_img.isNull());

    if (m_img.isNull()) {
        TLog::getInstance()->logIgnore("封面加载失败，加载链接为" + m_url);
    } else {
        m_img = m_img.scaled(m_requestedSize, Qt::IgnoreAspectRatio);
        buildRoundImage();
        ctr->writeImgCache(m_url, m_img);
    }

    emit finished();
}

QQuickImageResponse *ImageProvider::requestImageResponse(const QString &id, const QSize &requestedSize) {
    auto *res = new class ImageResponse(id, requestedSize);
    pool.start(res);
    return res;
}
