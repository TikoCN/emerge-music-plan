#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "setting.h"
#include "load/taskcenter.h"
#include "mediaplayer.h"
#include "online.h"
#include "base.h"
#include "musiccore.h"
#include "sqlite/sqlite.h"
#include "filemanagement.h"
#include "imageload/imagecontrol.h"
#include "imageload/imageprovider.h"
#include <QQmlContext>
#include <QIcon>
#include <QObject>
#include <QProcess>

SQLite *SQLite::instance = nullptr;
Setting* Setting::instance = nullptr;
TaskCenter* TaskCenter::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
OnLine* OnLine::instance = nullptr;
Base* Base::instance = nullptr;
MusicCore *MusicCore::instance = nullptr;
TLog *TLog::instance = nullptr;
FileManagement *FileManagement::instance = nullptr;
ImageControl *ImageControl::instance = nullptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/image/exe.png"));

    //建立
    TLog::buildInstance();
    MusicCore::buildInstance();
    MediaPlayer::buildInstance();
    Setting::buildInstance();

    FileManagement::buildInstance();
    TaskCenter::buildInstance();
    Base::buildInstance();
    SQLite::buildInstance();
    OnLine::buildInstance();
    ImageControl::buildInstance();

    //获得单例指针
    Setting *seit = Setting::getInstance();
    MusicCore *core = MusicCore::getInstance();
    MediaPlayer *mediaPlayer = MediaPlayer::getInstance();
    TaskCenter *center = TaskCenter::getInstance();
    OnLine *onLine = OnLine::getInstance();
    Base *base = Base::getInstance();
    SQLite *sql = SQLite::getInstance();
    FileManagement *fileMan = FileManagement::getInstance();
    ImageControl *imgCtr = ImageControl::getInstance();

    // 注册单例
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<Base>("MediaerAPI", 1, 0, "Base", base);
    qmlRegisterSingletonInstance<MusicCore>("MediaerAPI", 1, 0, "Core", core);
    qmlRegisterSingletonInstance<SQLite>("MediaerAPI", 1, 0, "SQLData", sql);
    qmlRegisterSingletonInstance<FileManagement>("MediaerAPI", 1, 0, "FileMan", fileMan);
    qmlRegisterSingletonInstance<ImageControl>("MediaerAPI", 1, 0, "ImageControl", imgCtr);

    // 注册数据类
    qmlRegisterType<LrcData>("DataType", 1, 0, "LrcData");

    QObject::connect(seit, &Setting::loadMusics, center, &TaskCenter::start);
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
    engine.loadFromModule("PlayView", "Main");
    engine.addImageProvider("cover", new ImageProvider);

    return app.exec();
}
