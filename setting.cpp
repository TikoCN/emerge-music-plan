#include "setting.h"
#include "base.h"
#include "mediaplayer.h"
#include "musiccore.h"
#include <QSettings>
#include <QDir>
#include <QGuiApplication>
#include <QScreen>
#include <QJsonDocument>

void Setting::loadMusicCores(){
    emit loadMusics();
}

/*
读取参数
*/
bool Setting::getParameterList()
{
    //读取成功
    if(!QFile::exists(QDir::currentPath() + "/setting.ini")){
        return false;
    }
    MediaPlayer* player = MediaPlayer::getInstance();
    QSettings *ini = new QSettings(QDir::currentPath() + "/setting.ini", QSettings::IniFormat);
    ini->beginGroup("seit");

    isOnLine = ini->value("isOnLine").toBool();
    isGetCoverFromNetEase = ini->value("isGetCoverFromNetEase").toBool();
    isGetCoverFromQQMusic = ini->value("isGetCoverFromQQMusic").toBool();
    isGetCoverFromBing = ini->value("isGetCoverFromBing").toBool();
    isGetCoverFromBaidu = ini->value("isGetCoverFromBaidu").toBool();
    isGetLrcFromNetEase = ini->value("isGetLrcFromNetEase").toBool();
    isGetLrcFromQQMusic = ini->value("isGetLrcFromQQMusic").toBool();

    sourceList = ini->value("sourceList").toStringList();

    maxThreadNumber = ini->value("maxThreadNumber").toInt();

    transparentColor = QColor::fromString(ini->value("transparentColor").toString());
    backdropColor = QColor::fromString(ini->value("backdropColor").toString());
    themeColor = QColor::fromString(ini->value("themeColor").toString());

    deskFont.fromString(ini->value("deskFont").toString());
    mainFont.fromString(ini->value("mainFont").toString());
    mainLrcFont.fromString(ini->value("mainLrcFont").toString());

    lrcTopPoint.setX(ini->value("lrcTopPointX").toDouble());
    lrcTopPoint.setY(ini->value("lrcTopPointY").toDouble());

    windowRect.setRect(ini->value("windowRectX").toDouble(), ini->value("windowRectY").toDouble(),
                       ini->value("windowRectW").toDouble(), ini->value("windowRectH").toDouble());
    player->audioOutput->setVolume(ini->value("volume").toFloat());

    ini->endGroup();
    ini->sync();//写入磁盘
    delete ini;

    //读取成功
    return true;
}

void Setting::removeUrl(QString url)
{
    int i = sourceList.indexOf(url);
    if(i >= 0){
        sourceList.remove(i);
    }

    emit Base::getInstance()->sendMessage(url + tr(" 路径移除成功，将在下次加载时生效"), 0);
}

template<typename T>
void Setting::setParameter(QString key, T value)
{
    QSettings *ini = new QSettings(QDir::currentPath() + "/setting.ini", QSettings::IniFormat);
    ini->beginGroup("seit");

    ini->setValue(key, value);

    ini->endGroup();
    ini->sync();//写入磁盘
    delete ini;
}

void Setting::writeData()
{
    MediaPlayer* player = MediaPlayer::getInstance();
    MusicCore* core = MusicCore::getInstance();
    QSettings *ini = new QSettings(QDir::currentPath() + "/setting.ini", QSettings::IniFormat);
    ini->beginGroup("seit");

    ini->setValue("isOnLine", isOnLine);
    ini->setValue("isGetCoverFromNetEase", isGetCoverFromNetEase);
    ini->setValue("isGetCoverFromQQMusic", isGetCoverFromQQMusic);
    ini->setValue("isGetCoverFromBing", isGetCoverFromBing);
    ini->setValue("isGetCoverFromBaidu", isGetCoverFromBaidu);
    ini->setValue("isGetLrcFromNetEase", isGetLrcFromNetEase);
    ini->setValue("isGetLrcFromQQMusic", isGetLrcFromQQMusic);

    ini->setValue("sourceList", sourceList);

    ini->setValue("maxThreadNumber", maxThreadNumber);

    ini->setValue("themeColor", themeColor.name());
    ini->setValue("transparentColor", transparentColor.name());
    ini->setValue("backdropColor", backdropColor.name());

    ini->setValue("mainFont", mainFont.toString());
    ini->setValue("mainLrcFont", mainLrcFont.toString());
    ini->setValue("deskFont", deskFont.toString());

    ini->setValue("lrcTopPointX", lrcTopPoint.x());
    ini->setValue("lrcTopPointY", lrcTopPoint.y());

    ini->setValue("windowRectX", windowRect.x());
    ini->setValue("windowRectY", windowRect.y());
    ini->setValue("windowRectW", windowRect.width());
    ini->setValue("windowRectH", windowRect.height());

    ini->setValue("volume", player->audioOutput->volume());

    ini->endGroup();
    ini->sync();//写入磁盘
    delete ini;

    QFile dataFile(QDir::currentPath() + "/data.json");
    QJsonObject writeData;
    if(dataFile.open(QIODevice::Text | QIODevice::WriteOnly)){
        //获得音乐核心json
        QJsonObject coreJson;
        QStringList nameList;
        for(int i=0; i<core->coreList.size(); i++){
            nameList.append(core->coreList[i]->getKey());
        }
        coreJson.insert("size", core->coreList.size());
        coreJson.insert("musicKeyList", nameList.join("||"));

        //获得列表json
        QJsonObject tableJson;
        for(int i=0; i<core->tableList.size(); i++){
            QJsonObject table;
            table.insert("sort", core->tableList[i]->getSort());
            table.insert("isDir", core->tableList[i]->isDir);
            table.insert("url", core->tableList[i]->url);
            table.insert("name", core->tableList[i]->name);
            table.insert("musicNumber", core->tableList[i]->musics.size());
            //插入列表的信息
            if(!core->tableList[i]->isDir){
                QStringList coreKey;
                for(int j=0; j<core->tableList[i]->musics.size(); j++){
                    coreKey.append(QString::number(core->tableList[i]->musics[j]->coreId));
                }
                table.insert("music", coreKey.join("||"));
            }

            tableJson.insert(QString::number(i), table);
        }
        tableJson.insert("size", core->tableList.size());

        writeData.insert("core", coreJson);
        writeData.insert("table", tableJson);
        QJsonDocument doc(writeData);
        dataFile.write(doc.toJson(QJsonDocument::Compact));
    }
    dataFile.close();
}

Setting::Setting()
{
    if(!getParameterList()){
        isOnLine = true;
        isGetCoverFromNetEase = true;
        isGetCoverFromQQMusic = true;
        isGetCoverFromBing = true;
        isGetCoverFromBaidu = true;
        isGetLrcFromNetEase = true;
        isGetLrcFromQQMusic = true;

        maxThreadNumber = 10;

        themeColor = Qt::red;
        transparentColor = Qt::black;
        backdropColor = Qt::white;

        mainFont.setFamily("Microsoft YaHei");
        mainFont.setPixelSize(12);
        mainLrcFont.setFamily("Microsoft YaHei");
        mainLrcFont.setPixelSize(12);
        mainLrcFont.setBold(1);
        deskFont.setFamily("Microsoft YaHei");
        deskFont.setPixelSize(15);
        deskFont.setBold(1);

        QRectF screen = QGuiApplication::primaryScreen()->geometry();
        lrcTopPoint.setX(screen.width()/2);
        lrcTopPoint.setY(screen.height()/2);

        windowRect.setRect(screen.width() * 0.2, screen.height() * 0.2,
                           screen.width() * 0.6, screen.height() * 0.6);

        writeData();
    }

    MediaPlayer* player = MediaPlayer::getInstance();
    //额外读写数据
    connect(player->audioOutput, &QAudioOutput::volumeChanged, this, [=](float volume){
        setParameter("volume", volume);
    });
}

bool Setting::getIsGetLrcFromQQMusic() const
{
    return isGetLrcFromQQMusic;
}

void Setting::setIsGetLrcFromQQMusic(bool newIsGetLrcFromQQMusic)
{
    if (isGetLrcFromQQMusic == newIsGetLrcFromQQMusic)
        return;
    isGetLrcFromQQMusic = newIsGetLrcFromQQMusic;
    emit isGetLrcFromQQMusicChanged();
    //
    setParameter("isGetLrcFromQQMusic", isGetLrcFromQQMusic);
}

bool Setting::getIsGetCoverFromQQMusic() const
{
    return isGetCoverFromQQMusic;
}

void Setting::setIsGetCoverFromQQMusic(bool newIsGetCoverFromQQMusic)
{
    if (isGetCoverFromQQMusic == newIsGetCoverFromQQMusic)
        return;
    isGetCoverFromQQMusic = newIsGetCoverFromQQMusic;
    emit isGetCoverFromQQMusicChanged();
    //
    setParameter("isGetCoverFromQQMusic", isGetCoverFromQQMusic);
}

bool Setting::getIsGetLrcFromNetEase() const
{
    return isGetLrcFromNetEase;
}

void Setting::setIsGetLrcFromNetEase(bool newIsGetLrcFromNetEase)
{
    if (isGetLrcFromNetEase == newIsGetLrcFromNetEase)
        return;
    isGetLrcFromNetEase = newIsGetLrcFromNetEase;
    emit isGetLrcFromNetEaseChanged();
    //
    setParameter("isGetLrcFromNetEase", isGetLrcFromNetEase);
}

bool Setting::getIsGetCoverFromBaidu() const
{
    return isGetCoverFromBaidu;
}

void Setting::setIsGetCoverFromBaidu(bool newIsGetCoverFromBaidu)
{
    if (isGetCoverFromBaidu == newIsGetCoverFromBaidu)
        return;
    isGetCoverFromBaidu = newIsGetCoverFromBaidu;
    emit isGetCoverFromBaiduChanged();
    //
    setParameter("isGetCoverFromBaidu", isGetCoverFromBaidu);

}

bool Setting::getIsGetCoverFromBing() const
{
    return isGetCoverFromBing;
}

void Setting::setIsGetCoverFromBing(bool newIsGetCoverFromBing)
{
    if (isGetCoverFromBing == newIsGetCoverFromBing)
        return;
    isGetCoverFromBing = newIsGetCoverFromBing;
    emit isGetCoverFromBingChanged();
    //
    setParameter("isGetCoverFromBing", isGetCoverFromBing);
}

bool Setting::getIsGetCoverFromNetEase() const
{
    return isGetCoverFromNetEase;
}

void Setting::setIsGetCoverFromNetEase(bool newIsGetCoverFromNetEase)
{
    if (isGetCoverFromNetEase == newIsGetCoverFromNetEase)
        return;
    isGetCoverFromNetEase = newIsGetCoverFromNetEase;
    emit isGetCoverFromNetEaseChanged();
    //
    setParameter("isGetCoverFromNetEase", isGetCoverFromNetEase);
}

bool Setting::getIsOnLine() const
{
    return isOnLine;
}

void Setting::setIsOnLine(bool newIsOnLine)
{
    if (isOnLine == newIsOnLine)
        return;
    isOnLine = newIsOnLine;
    emit isOnLineChanged();
    //
    setParameter("isOnLine", isOnLine);
}

QPoint Setting::getLrcTopPoint() const
{
    return lrcTopPoint;
}

void Setting::setLrcTopPoint(QPoint newLrcTopPoint)
{
    if (lrcTopPoint == newLrcTopPoint)
        return;
    lrcTopPoint = newLrcTopPoint;
    emit lrcTopPointChanged();
    setParameter("lrcTopPointX", lrcTopPoint.x());
    setParameter("lrcTopPointY", lrcTopPoint.y());
}

QRectF Setting::getWindowRect() const
{
    return windowRect;
}

void Setting::setWindowRect(const QRectF &newWindowRect)
{
    if (windowRect == newWindowRect)
        return;
    windowRect = newWindowRect;
    emit windowRectChanged();
    //写入设置
    setParameter("windowRectX", windowRect.x());
    setParameter("windowRectY", windowRect.y());
    setParameter("windowRectW", windowRect.width());
    setParameter("windowRectH", windowRect.height());
}

QFont Setting::getDeskFont() const
{
    return deskFont;
}

void Setting::setDeskFont(const QFont &newdeskFont)
{
    if (deskFont == newdeskFont)
        return;
    deskFont = newdeskFont;
    emit deskFontChanged();
    //写入设置文件
    setParameter("deskFont", deskFont.toString());
}

QFont Setting::getMainFont() const
{
    return mainFont;
}

void Setting::setMainFont(const QFont &newmainFont)
{
    if (mainFont == newmainFont)
        return;
    mainFont = newmainFont;
    emit mainFontChanged();
    //写入设置文件
    setParameter("mainFont", mainFont.toString());
}

QFont Setting::getMainLrcFont() const
{
    return mainLrcFont;
}

void Setting::setMainLrcFont(const QFont &newMainLrcFont)
{
    if (mainLrcFont == newMainLrcFont)
        return;
    mainLrcFont = newMainLrcFont;
    emit mainLrcFontChanged();
    //写入设置文件
    setParameter("mainLrcFont", mainLrcFont.toString());
}

QColor Setting::getBackdropColor() const
{
    return backdropColor;
}

void Setting::setBackdropColor(const QColor &newbackdropColor)
{
    if (backdropColor == newbackdropColor)
        return;
    backdropColor = newbackdropColor;
    emit backdropColorChanged();
    //写入设置文件
    setParameter("backdropColor", backdropColor);
}

QColor Setting::getTransparentColor() const
{
    return transparentColor;
}

void Setting::setTransparentColor(const QColor &newtransparentColor)
{
    if (transparentColor == newtransparentColor)
        return;
    transparentColor = newtransparentColor;
    emit transparentColorChanged();
    //写入设置文件
    setParameter("transparentColor", transparentColor);
}

QColor Setting::getThemeColor() const
{
    return themeColor;
}

void Setting::setThemeColor(const QColor &newthemeColor)
{
    if (themeColor == newthemeColor)
        return;
    themeColor = newthemeColor;
    emit themeColorChanged();
    //写入设置文件
    setParameter("themeColor", themeColor);
}

int Setting::getMaxThreadNumber() const
{
    return maxThreadNumber;
}

void Setting::setMaxThreadNumber(int newMaxThreadNumber)
{
    if (maxThreadNumber == newMaxThreadNumber)
        return;
    maxThreadNumber = newMaxThreadNumber;
    emit maxThreadNumberChanged();
    //写入设置文件
    setParameter("maxThreadNumber", maxThreadNumber);
}

QStringList Setting::getSourceList() const
{
    return sourceList;
}

void Setting::setSourceList(const QStringList &newSourceList)
{
    if (sourceList == newSourceList)
        return;
    sourceList = newSourceList;
    emit sourceListChanged();
    //写入设置文件
    setParameter("sourceList", sourceList);
}
