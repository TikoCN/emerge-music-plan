#include "music.h"
#include "ffmpeg.h"
#include "base.h"
#include "extralibrary.h"
#include <QDesktopServices>
#include <QJsonObject>
#include <QGuiApplication>
#include <QClipboard>
#include <QImageReader>
#include <QPixmap>

Music::Music() {
    love = false;
    maxHeight = 0.0;
    minHeight = 0.0;
    playNumber = 0;
    url = "";
}

void Music::writeDataToFile(QString lrc, QString title, QString artist, QString alumb, QString genre, QString year)
{
    Base *base = Base::getInstance();
    QString lrcUrl = getLrcUrl();
    bool r = base->writeFileText(lrcUrl, lrc);
    if(r){
        emit base->sendMessage(lrcUrl + tr(" 修改歌词文件成功"), 0);
    }
    else{
        emit base->sendMessage(lrcUrl + tr(" 修改歌词文件失败"), 1);
    }

    ExtraLibrary ex;
    r = ex.setMedia(url, title, artist, alumb, genre, year);
    if(r){
        emit base->sendMessage(url + tr(" 修改音乐文件属性成功"), 0);
    }
    else{
        emit base->sendMessage(url + tr(" 修改音乐文件属性失败"), 1);
    }

    setTitle(title);
    setArtist(artist);
    setAlumb(alumb);
    setGenre(genre);
    setYear(year);
}

void Music::fromFileInfo(QFileInfo info)
{
    url = info.filePath();
    lastEdit = info.lastModified().toString("yy-MM-dd hh:mm:ss");
    lastEditTime = info.lastModified().toMSecsSinceEpoch();
}
/*
 * 来自Json
*/
void Music::fromJson(QJsonObject obj)
{
    playNumber = obj.value("playNumber").toInt();
    love = obj.value("love").toBool();
}

//获得封面路径
QString Music::getCoverUrl()
{
    return url.split("." + url.split(".").last())[0] + ".jpg";
}

QString Music::getLrcUrl()
{
    return url.split("." + url.split(".").last())[0] + ".lrc";
}

/*
 * 获得父文件夹
 */
QString Music::getParentDir()
{
    QString fileName = url.split("/").last();
    return url.split("/"+fileName)[0];
}

QString Music::getBaseName()
{
    return url.split("/").last().split("." + url.split(".").last())[0];
}

/*
 * 时长
 */
QString Music::getStringTime()
{
    //qint64 ms = endTime % 1000;
    qint64 s = endTime / 1000 % 60;
    qint64 m = endTime / 1000 / 60;
    return QString::number(m) + ":" + QString::number(s);
}

/*
 * 获得识别文本
 */
QString Music::getKey()
{
    return title +"-&&-"+ artist;
}

QString Music::getSearchString()
{
    QString search;
    if(!title.isNull() || !artist.isNull()){
        search = title + artist;
    }
    else{
        search = getBaseName();
    }

    return search;
}

/*
加载封面
*/
QPixmap Music::loadCover()
{
    FFmpeg ffmpeg;

    QString coverUrl = getCoverUrl();
    //提取附加封面
    QImage img = ffmpeg.getInlayCover(url);

    if(img.isNull()){
        //如果存在 独立封面
        if(QFile::exists(coverUrl)){
            img = loadAloneCover();
        }
    }

    return QPixmap::fromImage(img);
}

/*
 * 加载独立的封面文件
*/
QImage Music::loadAloneCover()
{
    QImage img;
    QString coverUrl = getCoverUrl();
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
    if(title.contains(aim) || artist.contains(aim) || alumb.contains(aim)){
        return true;
    }
    return false;
}

QString Music::getLrcData()
{
    return Base::getInstance()->readFileText(getLrcUrl());
}

/*
 * 复制音乐信息
*/
void Music::copyMusicData()
{
    QString data = tr("标题") +":"+ title +" "
                   +tr("歌手") +":"+ artist +" "
                   +tr("专辑") +":"+ alumb;
    Base::getInstance()->copyString(data);
}

/*
 * 复制音乐文件路径
*/
void Music::copyMusicUrl()
{
    Base::getInstance()->copyString(url);
}

/*
 * 打开音乐文件夹
*/
void Music::openMusicDir()
{
    Base::getInstance()->deskOpenFile(getParentDir(), 1);
}

/*
 * 打开音乐封面文件
*/
void Music::openMusicCover()
{
    Base::getInstance()->deskOpenFile(getCoverUrl(), 1);
}

/*
 * 打开音乐歌词文件
*/
void Music::openMusicLrc()
{
    Base::getInstance()->deskOpenFile(getLrcUrl(), 1);
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
    qDebug()<<s;
}

QString Music::getTitle() const
{
    return title;
}

void Music::setTitle(const QString &newTitle)
{
    if (title == newTitle)
        return;
    title = newTitle;
    emit titleChanged();
}

QString Music::getArtist() const
{
    return artist;
}

void Music::setArtist(const QString &newArtist)
{
    if (artist == newArtist)
        return;
    artist = newArtist;
    emit artistChanged();
}

QString Music::getUrl() const
{
    return url;
}

void Music::setUrl(const QString &newUrl)
{
    if (url == newUrl)
        return;
    url = newUrl;
    emit urlChanged();
}

QString Music::getAlumb() const
{
    return alumb;
}

void Music::setAlumb(const QString &newAlumb)
{
    if (alumb == newAlumb)
        return;
    alumb = newAlumb;
    emit alumbChanged();
}

QString Music::getLastEdit() const
{
    return lastEdit;
}

void Music::setLastEdit(const QString &newLastEdit)
{
    if (lastEdit == newLastEdit)
        return;
    lastEdit = newLastEdit;
    emit lastEditChanged();
}

int Music::getCoreId() const
{
    return coreId;
}

void Music::setCoreId(int newCoreId)
{
    if (coreId == newCoreId)
        return;
    coreId = newCoreId;
    emit coreIdChanged();
}

bool Music::getLove() const
{
    return love;
}

void Music::setLove(bool newLove)
{
    if (love == newLove)
        return;
    love = newLove;
    emit loveChanged();
}

QString Music::getGenre() const
{
    return genre;
}

void Music::setGenre(const QString &newGenre)
{
    if (genre == newGenre)
        return;
    genre = newGenre;
    emit genreChanged();
}

QString Music::getYear() const
{
    return year;
}

void Music::setYear(const QString &newYear)
{
    if (year == newYear)
        return;
    year = newYear;
    emit yearChanged();
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
}
