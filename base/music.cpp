#include "music.h"
#include "ffmpeg.h"
#include "base.h"
#include "lrcdata.h"
#include <QDesktopServices>
#include <QJsonObject>
#include <QGuiApplication>
#include <QClipboard>
#include <QImageReader>
#include <QPixmap>
#include "mediadata.h"
#include "sqlite/sqlite.h"

QJsonObject Music::getJsonObject()
{
<<<<<<< Updated upstream:base/music.cpp
    return isLove;
}

void Music::setIsLove(bool newIsLove)
{
    if (isLove == newIsLove)
        return;
    isLove = newIsLove;
    emit isLoveChanged();

    SQLite::getInstance()->updateMusic(this);
}

int Music::getLevel() const
{
    return level;
}

void Music::setLevel(int newLevel)
{
    if (level == newLevel)
        return;
    level = newLevel;
    emit levelChanged();

    SQLite::getInstance()->updateMusic(this);
}

long long Music::getDuration() const
{
    return duration;
}

int Music::getPlayNumber() const
{
    return playNumber;
}

void Music::setPlayNumber(int newPlayNumber)
{
    if (playNumber == newPlayNumber)
        return;
    playNumber = newPlayNumber;
    emit playNumberChanged();

    SQLite::getInstance()->updateMusic(this);
=======
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
>>>>>>> Stashed changes:qt/base/music.cpp
}

Music::Music() {
    url = "";
    album = QObject::tr("未知专辑");
    artistList.append(QObject::tr("未知歌手"));
    title = QObject::tr("未知音乐");
    level = 0;
    isLove = false;
    playNumber = 0;
<<<<<<< Updated upstream:base/music.cpp
}

void Music::writeDataToFile(QStringList key, QStringList value)
{
    QString baseName = Base::getInstance()->getFileName(url);
    QString newUrl = url.replace(baseName, "new"+baseName);
    FFmpeg ff;
    if(ff.writeDict(key, value, url, newUrl)){
        Base::getInstance()->writeFileText(url, newUrl);
    }
    else{
       Base::getInstance()->sendMessage(tr("写入歌曲信息失败"), 1);
    }
=======
>>>>>>> Stashed changes:qt/base/music.cpp
}

void Music::setMedia(MediaData data)
{
    id = data.id;
    title = data.title;
    artistList = data.artistList;
    album = data.album;
    playNumber = data.playNumber;
    isLove = data.isLove;
    url = data.url;
    level = data.level;
    insetTime = data.insetTime;
}


QString Music::getMediaJson()
{
    return "";
}

void Music::fromFileInfo(QFileInfo info)
{
    url = info.filePath();
    lastEdit = info.lastModified().toString("yy-MM-dd hh:mm:ss");
    lastEditTime = info.lastModified().toMSecsSinceEpoch();
}

/*
加载封面
*/
QImage Music::loadCover(QString url)
{
    FFmpeg ffmpeg;

    QString coverUrl = Base::getInstance()->getBaseUrl(url) + ".jpg";
    //提取附加封面
    QImage img = ffmpeg.getInlayCover(url);

    if(img.isNull()){
        //如果存在 独立封面
        if(QFile::exists(coverUrl)){
            img = loadAloneCover(url);
        }
    }

    return img;
}

/*
 * 加载独立的封面文件
*/
QImage Music::loadAloneCover(QString url)
{
    QImage img;
    QString coverUrl = Base::getInstance()->getBaseUrl(url) + ".jpg";

    //如果存在 独立封面
    if(QFile::exists(coverUrl))
    {
        QImageReader reader;
        reader.setFileName(coverUrl);
        QSize aim = reader.size();
        aim.scale(QSize(300,300), Qt::KeepAspectRatioByExpanding);
        reader.setScaledSize(aim);
        img = reader.read();
    }
    return img;
}

/*
 * 判断是否符合搜索条件
*/
bool Music::isSearch(QString aim)
{
    if(title.contains(aim) || artistList.contains(aim) || album.contains(aim)){
        return true;
    }
    return false;
}

/*
 * 格式转换
*/
void Music::setSuffix(QString type)
{
    FFmpeg ffmpeg;
    FFmpeg::Suffix suffix = FFmpeg::MP3;
    QStringList list = {"MP3", "FLAC", "ALAC", "AAC", "WMA", "PCM16", "PCM32"};
    switch (list.indexOf(type)) {
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
    bool s = false;
    s = ffmpeg.transformCodec(url, suffix);
}

QString Music::getArtist() const
{
    return artistList.join(";");
}

