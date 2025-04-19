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

bool Music::getIsLove() const
{
    return isLove;
}

void Music::setIsLove(bool newIsLove)
{
    if (isLove == newIsLove)
        return;
    isLove = newIsLove;
    emit isLoveChanged();
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
}

long long Music::getEndTime() const
{
    return endTime;
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

Music::Music() {
    url = "";
    album = tr("未知专辑");
    artistList.append(tr("未知歌手"));
    title = tr("未知音乐");
    level = 0;
    isLove = false;
    playNumber = 0;
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
}

void Music::readMedia()
{
    FFmpeg ff;
    MediaData data;
    ff.getDict(&data, url);
    setMedia(data);
}

void Music::setMedia(MediaData data)
{
    title = data.title;
    artistList = data.artistList;
    album = data.album;
    playNumber = data.playNumber;
    isLove = data.isLove;
    url = data.url;
    level = data.level;
    insetTime = data.insetTime;
}

QList<KeyValuePair *> Music::getAllKey()
{
    QList<KeyValuePair *> dict;
    QStringList key, value;
    FFmpeg ff;
    ff.getDict(&key, &value, url);
    for (int i = 0; i < key.size(); ++i) {
        KeyValuePair *data = new KeyValuePair(key[i], value[i]);
        dict.append(data);
    }
    return dict;
}

void Music::fromFileInfo(QFileInfo info)
{
    url = info.filePath();
    lastEdit = info.lastModified().toString("yy-MM-dd hh:mm:ss");
    lastEditTime = info.lastModified().toMSecsSinceEpoch();
}

QString Music::getLrcUrl()
{
    QString lrc = url.split("." + url.split(".").last())[0];
    QString hlrc = lrc + ".hlrc";

    if(QFile::exists(hlrc)){
        lrc = hlrc;
    }
    else {
        lrc += ".lrc";
    }
    return lrc;
}

QList<LrcData *> Music::getLyricsData()
{
    QString lrc = getLrcUrl();
    QList<LrcData *> lrcList;

    QFile lrcFile(lrc);
    if(!lrcFile.open(QIODevice::Text |QIODevice::ReadOnly)){
        return lrcList;
    }

    QTextStream in(&lrcFile);
    QRegularExpression rx;
    QRegularExpressionMatch match;
    QString line;
    LrcData *lrcD = nullptr;

    //读取高级歌词
    if(lrc.split(".").last() == "hlrc"){
        while(!in.atEnd()){
            line = in.readLine();

            //捕获开始时间和结束时间
            rx.setPattern(R"(\[(\d+),(\d+)\])");
            match = rx.match(line);
            //初始化并设置开始结束时间
            if(match.isValid()){
                lrcD = new LrcData;
                lrcList.append(lrcD);
                lrcD->id = lrcList.size()-1;
                lrcD->startTime = match.captured(1).toLong();
                lrcD->endTime = match.captured(2).toLong();
            }
            else{//没有发现行头，下一行
                continue;
            }

            //捕获主体
            QStringList lrcText = line.split("/");
            // 添加到其他文本
            for (int i = 1; i < lrcText.size(); ++i) {
                lrcD->helpTextList.append(lrcText[i]);
            }

            rx.setPattern(R"(\((\d+),(\d+)\)\s*([^(]*))");
            QRegularExpressionMatchIterator it = rx.globalMatch(lrcText.first());
            while(it.hasNext()){
                match = it.next();
                long long start = match.captured(1).toLong();
                long long end = match.captured(2).toLong();
                QString text = match.captured(3);
                lrcD->append(start, end, text);
            }
        }
    }
    else{
        rx.setPattern(R"(\[(\d+):(\d+).(\d+)\]([\s\S]*))");
        QStringList lrcTextList;
        //读取基本数据以及文本行
        while (!in.atEnd()) {
            line = in.readLine();
            match = rx.match(line);
            if(match.capturedTexts().size() == 5){
                lrcD = new LrcData;
                lrcD->id = lrcList.size();
                lrcD->startTime = match.captured(1).toLong() * 60 * 1000 +
                                  match.captured(2).toLong() * 1000 +
                                  match.captured(3).toLong();
                QStringList lrcText = match.captured(4).split("/");
                // 添加到其他文本
                for (int i = 1; i < lrcText.size(); ++i) {
                    lrcD->helpTextList.append(lrcText[i]);
                }

                lrcTextList.append(lrcText.first());
                lrcList.append(lrcD);
            }
            else{
                continue;
            }
        }

        //设置逐字时间戳
        for(int i=0; i<lrcList.size(); i++){
            long long start = lrcList[i]->startTime;
            long long end;
            if(i == lrcList.size() - 1){
                end = this->endTime;
            }
            else{
                end = lrcList[i+1]->startTime;
            }

            lrcList[i]->endTime = end;
            int length = lrcTextList[i].size() == 0 ? 1 : lrcTextList[i].size();
            int wordTime = (end - start) / length;

            //逐字遍历
            QString text = lrcTextList[i];
            for(int j=0; j<text.size(); j++){
                lrcList[i]->append(start + j * wordTime,
                                   start + (j + 1) * wordTime,
                                   text[j]);
            }
        }
    }

    return lrcList;
}

/*
 * 时长
 */
QString Music::getStringTime()
{
    QDateTime dataTime;
    dataTime.setMSecsSinceEpoch(endTime);
    dataTime = dataTime.toUTC();
    return dataTime.toString("mm:ss");
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
 * 复制音乐信息
*/
void Music::getString()
{
    QString data = QObject::tr("标题") +":"+ title +" "
                   +QObject::tr("歌手") +":"+ artistList.join(";") +" "
                   +QObject::tr("专辑") +":"+ album;
    Base::getInstance()->copyString(data);
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

QString Music::getTitle() const
{
    return title;
}

QString Music::getArtist() const
{
    return artistList.join(";");
}

QString Music::getUrl() const
{
    return url;
}

QString Music::getAlbum() const
{
    return album;
}

QString Music::getLastEdit() const
{
    return lastEdit;
}

int Music::getCoreId() const
{
    return coreId;
}

