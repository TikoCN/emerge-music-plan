#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "setting.h"
#include "hosttime.h"
#include "mediaplayer.h"
#include "imageprovider.h"
#include "online.h"
#include "base.h"
#include <QQmlContext>
#include <QIcon>

Setting* Setting::instance = nullptr;
HostTime* HostTime::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
OnLine* OnLine::instance = nullptr;
Base* Base::instance = nullptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/image/exe.png"));

    //建立
    OnLine::buildInstance();
    MediaPlayer::buildInstance();
    Setting::buildInstance();
    HostTime::buildInstance();
    Base::buildInstance();

    //获得单例指针
    Setting* seit = Setting::getInstance();
    MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
    HostTime* hostTime = HostTime::getInstance();
    OnLine* onLine = OnLine::getInstance();
    Base* base = Base::getInstance();

    qmlRegisterSingletonInstance("TikoAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance("TikoAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance("TikoAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance("TikoAPI", 1, 0, "Base", base);

    QObject::connect(hostTime, &HostTime::musicsLoaded, mediaPlayer, &MediaPlayer::getMusicCore);

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Widget", "Main");
    engine.addImageProvider("cover", new ImageProvider);

    return app.exec();
}
