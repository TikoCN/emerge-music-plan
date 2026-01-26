#include "music.h"
#include "ffmpeg.h"
#include "basetool/basetool.h"
#include <QDesktopServices>
#include <QImageReader>
#include <QPixmap>
#include "mediadata.h"
#include "tlog.h"

QJsonObject Music::getJsonObject() const {
    QJsonObject json;
    json.insert("title", title);
    json.insert("album", album);
    json.insert("artist", getArtist());
    json.insert("level", level);
    json.insert("isLove", isLove);
    json.insert("playNumber", playNumber);
    json.insert("url", url);
    json.insert("lastEdit", lastEdit);
    json.insert("duration", duration);
    return json;
}

Music::Music()
    : title(QObject::tr("未知音乐"))
      , artistList{QObject::tr("未知歌手")}
      , url("")
      , album(QObject::tr("未知专辑"))
      , level(0)
      , playNumber(0)
      , isLove(false) {
}

void Music::setMedia(const MediaData &data) {
    title = data.title;
    artistList = data.artistList;
    album = data.album;
    playNumber = data.playNumber;
    isLove = data.isLove;
    url = data.url;
    level = data.level;
}


QString Music::getMediaJson() {
    return "";
}

void Music::fromFileInfo(const QFileInfo &info) {
    url = info.filePath();
    lastEdit = info.lastModified().toString("yy-MM-dd hh:mm:ss");
    lastEditTime = info.lastModified().toMSecsSinceEpoch();
}

/*
加载封面
*/
QImage Music::loadCover(const QString &url) {
    FFmpeg ffmpeg;

    const QString coverUrl = FileManagement::getBaseUrl(url) + ".jpg";
    //提取附加封面
    QImage img = ffmpeg.getInlayCover(url);

    if (img.isNull()) {
        //如果存在 独立封面
        if (QFile::exists(coverUrl)) {
            img = loadAloneCover(url);
        }
    }

    return img;
}

/*
 * 加载独立的封面文件
*/
QImage Music::loadAloneCover(const QString &url) {
    QImage img;

    //如果存在 独立封面
    if (const QString coverUrl = FileManagement::getBaseUrl(url) + ".jpg"; QFile::exists(coverUrl)) {
        QImageReader reader;
        reader.setFileName(coverUrl);
        QSize aim = reader.size();
        aim.scale(QSize(300, 300), Qt::KeepAspectRatioByExpanding);
        reader.setScaledSize(aim);
        img = reader.read();
    }
    return img;
}

/*
 * 判断是否符合搜索条件
*/
bool Music::isSearch(const QString &aim) const {
    if (title.contains(aim) || artistList.contains(aim) || album.contains(aim)) {
        return true;
    }
    return false;
}

/*
 * 格式转换
*/
void Music::setSuffix(const QString &type) const {
    FFmpeg ffmpeg;
    FFmpeg::Suffix suffix = FFmpeg::MP3;
    switch (const QStringList list = {"MP3", "FLAC", "ALAC", "AAC", "WMA", "PCM16", "PCM32"}; list.indexOf(type)) {
        case 0:
            suffix = FFmpeg::MP3;
            break;
        case 1:
            suffix = FFmpeg::FLAC;
            break;
        case 2:
            suffix = FFmpeg::ALAC;
            break;
        case 3:
            suffix = FFmpeg::AAC;
            break;
        case 4:
            suffix = FFmpeg::WMA;
            break;
        case 5:
            suffix = FFmpeg::PCM16;
            break;
        case 6:
            suffix = FFmpeg::PCM32;
            break;
        default:
            break;
    }
    if (ffmpeg.transformCodec(url, suffix)) {
        TLog::getInstance()->logInfo(QObject::tr("转换完成"));
    }
}

QString Music::getArtist() const {
    return artistList.join(";");
}
