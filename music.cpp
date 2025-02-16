#include "music.h"
#include "extralibrary.h"
#include "setting.h"
#include "popupdata.h"
#include "online.h"
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

/*
加载封面
*/
QPixmap Music::loadCover()
{
    ExtraLibrary extraLibrary;

    QString coverUrl = getCoverUrl();
    QImage img = extraLibrary.loadIndexCover(url);
    if(img.isNull()){
        //如果存在 独立封面
        if(QFile::exists(coverUrl)){
            img = loadAloneCover();
        }
    }

    if(img.isNull()){
        img.load(":/image/default.jpg");
    }

    return QPixmap::fromImage(img);
}

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

/*
 * 复制音乐信息
*/
void Music::copyMusicData()
{
    QString data = tr("标题") +":"+ title +" "
                   +tr("歌手") +":"+ artist +" "
                   +tr("专辑") +":"+ alumb;
    QClipboard* copy = QGuiApplication::clipboard();
    copy->setText(data);
}

/*
 * 复制音乐文件路径
*/
void Music::copyMusicUrl()
{
    QClipboard* copy = QGuiApplication::clipboard();
    copy->setText(url);
}

/*
 * 打开音乐文件夹
*/
void Music::openMusicDir()
{
    QString url = "file:///" + getParentDir();
    QDesktopServices::openUrl(QUrl(url));
}

/*
 * 打开音乐封面文件
*/
void Music::openMusicCover()
{
    QString url = "file:///" + getCoverUrl();
    if(QFile::exists(getCoverUrl())){
        QDesktopServices::openUrl(QUrl(url));
    }
    else{
        PopupData::getInstance()->message(tr("音乐封面不存在") + url);
    }
}

/*
 * 打开音乐歌词文件
*/
void Music::openMusicLrc()
{
    QString url = "file:///" + getLrcUrl();
    if(QFile::exists(getLrcUrl())){
        QDesktopServices::openUrl(QUrl(url));
    }
    else{
        PopupData::getInstance()->message(tr("歌词文件不存在") + url);
    }
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
