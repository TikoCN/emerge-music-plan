#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "setting.h"
#include "load/taskcenter.h"
#include "mediaplayer.h"
#include "imageprovider.h"
#include "online.h"
#include "base.h"
#include "musiccore.h"
#include <QQmlContext>
#include <QIcon>

Setting* Setting::instance = nullptr;
TaskCenter* TaskCenter::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
OnLine* OnLine::instance = nullptr;
Base* Base::instance = nullptr;
MusicCore *MusicCore::instance = nullptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/image/exe.png"));

    //建立
    OnLine::buildInstance();
    MediaPlayer::buildInstance();
    Setting::buildInstance();
    TaskCenter::buildInstance();
    Base::buildInstance();
    MusicCore::buildInstance();

    //获得单例指针
    Setting* seit = Setting::getInstance();
    MusicCore* core = MusicCore::getInstance();
    MediaPlayer* mediaPlayer = MediaPlayer::getInstance();
    TaskCenter* center = TaskCenter::getInstance();
    OnLine* onLine = OnLine::getInstance();
    Base* base = Base::getInstance();
    mediaPlayer->core = core;

    // 注册单例
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<Base>("MediaerAPI", 1, 0, "Base", base);
    qmlRegisterSingletonInstance<MusicCore>("MediaerAPI", 1, 0, "Core", core);

    // 注册数据类
    qmlRegisterType<Music *>("DataCore", 1, 0, "MusicData");
    qmlRegisterType<Table *>("DataCore", 1, 0, "TableData");
    qmlRegisterType<LrcData *>("DataCore", 1, 0, "LrcData");
    qmlRegisterType<Artist *>("DataCore", 1, 0, "ArtistData");
    qmlRegisterType<Alumb *>("DataCore", 1, 0, "AlumbData");

    QObject::connect(center, &TaskCenter::musicsLoaded, core, &MusicCore::getMusicCore);
    QObject::connect(mediaPlayer, &MediaPlayer::downLrc, onLine, &OnLine::downLrc);
    QObject::connect(onLine, &OnLine::lrcDowned, mediaPlayer, &MediaPlayer::loadLrcList);

    // 开始加载
    seit->loadMusicCores();

    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("Ups", "Main");
    engine.addImageProvider("cover", new ImageProvider);

    return app.exec();
}
