#include "setting.h"
#include "mediaplay/mediaplayer.h"
#include <QSettings>
#include <QDir>
#include <QScreen>

void Setting::loadMusicCores(){
    emit loadMusics();
}

/*
读取参数
*/
bool Setting::getParameterList()
{
    //读取成功
    if(!QFile::exists(m_iniUrl)){
        return false;
    }
    const MediaPlayer* player = MediaPlayer::getInstance();
    auto *ini = new QSettings(m_iniUrl,QSettings::IniFormat);
    ini->beginGroup("seit");

    m_isOnLine = ini->value(m_isOnLineKey).toBool();
    m_isGetCoverFromNetEase = ini->value(m_isGetCoverFromNetEaseKey).toBool();
    m_isGetCoverFromQQMusic = ini->value(m_isGetCoverFromQQMusicKey).toBool();
    m_isGetCoverFromBing = ini->value(m_isGetCoverFromBingKey).toBool();
    m_isGetCoverFromBaidu = ini->value(m_isGetCoverFromBaiduKey).toBool();
    
    m_isGetLrcFromNetEase = ini->value(m_isGetLrcFromNetEaseKey).toBool();
    m_isGetLrcFromQQMusic = ini->value(m_isGetLrcFromQQMusicKey).toBool();

    m_sourceList = ini->value(m_sourceListKey).toStringList();

    m_maxThreadNumber = ini->value(m_maxThreadNumberKey).toInt();

    m_transparentColor = QColor::fromString(ini->value(m_transparentColorKey).toString());
    m_backdropColor = QColor::fromString(ini->value(m_backdropColorKey).toString());
    m_themeColor = QColor::fromString(ini->value(m_themeColorKey).toString());
    m_deskLrcColor = QColor::fromString(ini->value(m_deskLrcColorKey).toString());

    m_deskFont.fromString(ini->value(m_deskFontKey).toString());
    m_mainFont.fromString(ini->value(m_mainFontKey).toString());
    m_mainLrcFont.fromString(ini->value(m_mainLrcFontKey).toString());

    m_lrcTopPoint = ini->value(m_lrcTopPointKey).toPoint();

    m_windowRect = ini->value(m_windowRectKey).toRectF();
    player->getAudioOutput()->setVolume(ini->value("volume").toFloat());

    ini->endGroup();
    ini->sync();//写入磁盘
    delete ini;

    //读取成功
    return true;
}

void Setting::removeUrl(const QString &url)
{
    if(const long long i = m_sourceList.indexOf(url); i >= 0){
        m_sourceList.remove(i);
    }

    //todo emit Base::getInstance()->sendMessage(url + tr(" 路径移除成功，将在下次加载时生效"), 0);
}

void Setting::writeData() const
{
    const MediaPlayer* player = MediaPlayer::getInstance();
    auto *ini = new QSettings(m_iniUrl,QSettings::IniFormat);
    ini->beginGroup("seit");

    ini->setValue(m_isOnLineKey,m_isOnLine);
    ini->setValue(m_isGetCoverFromNetEaseKey,m_isGetCoverFromNetEase);
    ini->setValue(m_isGetCoverFromQQMusicKey,m_isGetCoverFromQQMusic);
    ini->setValue(m_isGetCoverFromBingKey,m_isGetCoverFromBing);
    ini->setValue(m_isGetCoverFromBaiduKey,m_isGetCoverFromBaidu);
    ini->setValue(m_isGetLrcFromNetEaseKey,m_isGetLrcFromNetEase);
    ini->setValue(m_isGetLrcFromQQMusicKey,m_isGetLrcFromQQMusic);

    ini->setValue(m_sourceListKey,m_sourceList);

    ini->setValue(m_maxThreadNumberKey,m_maxThreadNumber);

    ini->setValue(m_themeColorKey,m_themeColor);
    ini->setValue(m_transparentColorKey,m_transparentColor);
    ini->setValue(m_backdropColorKey,m_backdropColor);
    ini->setValue(m_deskLrcColorKey,m_deskLrcColor);

    ini->setValue(m_mainFontKey,m_mainFont);
    ini->setValue(m_mainLrcFontKey,m_mainLrcFont);
    ini->setValue(m_deskFontKey,m_deskFont);

    ini->setValue(m_lrcTopPointKey,m_lrcTopPoint.x());

    ini->setValue(m_windowRectKey,m_windowRect);

    ini->setValue("volume",player->getAudioOutput()->volume());

    ini->endGroup();
    ini->sync();//写入磁盘
    delete ini;
}

Setting::Setting()
    :m_iniUrl(QDir::currentPath() + "/data/setting.ini")
{
    if(!getParameterList()){
        m_isOnLine = true;
        m_isGetCoverFromNetEase = true;
        m_isGetCoverFromQQMusic = true;
        m_isGetCoverFromBing = true;
        m_isGetCoverFromBaidu = true;
        m_isGetLrcFromNetEase = true;
        m_isGetLrcFromQQMusic = true;

        m_maxThreadNumber = 10;

        m_themeColor = Qt::red;
        m_transparentColor = Qt::black;
        m_backdropColor = Qt::white;
        m_deskLrcColor = Qt::red;

        m_mainFont.setFamily("Microsoft YaHei");
        m_mainFont.setPixelSize(12);
        m_mainLrcFont.setFamily("Microsoft YaHei");
        m_mainLrcFont.setPixelSize(18);
        m_mainLrcFont.setBold(true);
        m_deskFont.setFamily("Microsoft YaHei");
        m_deskFont.setPixelSize(20);
        m_deskFont.setBold(true);

        const QRectF screen = QGuiApplication::primaryScreen()->geometry();
        m_windowRect.setRect(screen.width() * 0.2, screen.height() * 0.2,
                           screen.width() * 0.6, screen.height() * 0.6);
        m_lrcTopPoint.setX(m_windowRect.x());
        m_lrcTopPoint.setY(m_windowRect.y() + m_windowRect.height()/2);

        writeData();
    }

    const MediaPlayer* player = MediaPlayer::getInstance();
    //额外读写数据
    connect(player->getAudioOutput(), &QAudioOutput::volumeChanged, this, [=](const float volume){
        setParameter("volume",volume);
    });

    connect(player->getPlayer(), &QMediaPlayer::playbackRateChanged, this, [=](const float rate){
        setParameter("playRate",rate);
    });
}