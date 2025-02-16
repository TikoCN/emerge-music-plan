#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "setting.h"
#include "hosttime.h"
#include "mediaplayer.h"
#include "imageprovider.h"
#include "popupdata.h"
#include "toolhelper.h"
#include "online.h"
#include <QQmlContext>
#include <QIcon>

Setting* Setting::instance = nullptr;
HostTime* HostTime::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
PopupData* PopupData::instance = nullptr;
ToolHelper* ToolHelper::instance = nullptr;
OnLine* OnLine::instance = nullptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/image/exe.png"));

    //建立
    OnLine::buildInstance();
    MediaPlayer::buildInstance();
    Setting::buildInstance();
    HostTime::buildInstance();
    PopupData::buildInstance();
    ToolHelper::buildInstance();

    //获得单例指针
    Setting* seit = Setting::getInstance();
    MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
    HostTime* hostTime = HostTime::getInstance();
    PopupData* popupData = PopupData::getInstance();
    ToolHelper* toolHelper = ToolHelper::getInstance();

    qmlRegisterSingletonInstance("MyAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance("MyAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance("MyAPI", 1, 0, "PopupData", popupData);
    qmlRegisterSingletonInstance("MyAPI", 1, 0, "ToolHelper", toolHelper);

    QObject::connect(hostTime, &HostTime::musicsLoaded, mediaPlayer, &MediaPlayer::getMusicCore);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("musicPlay", "Main");
    engine.addImageProvider("cover", new ImageProvider);

    return app.exec();
}
