#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QObject>
#include <QProcess>

#include "setting.h"
#include "load/taskcenter.h"
#include "mediaplayer.h"
#include "online.h"
#include "base.h"
#include "datacore/dataactive.h"
#include "datacore/dataactive.h"
#include "sqlite/sqlite.h"
#include "filemanagement.h"
#include "imageload/imagecontrol.h"
#include "imageload/imageprovider.h"

SQLite *SQLite::instance = nullptr;
Setting* Setting::instance = nullptr;
TaskCenter* TaskCenter::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
DataActive* DataActive::instance = nullptr;
OnLine* OnLine::instance = nullptr;
Base* Base::instance = nullptr;
TLog *TLog::instance = nullptr;
FileManagement *FileManagement::instance = nullptr;
ImageControl *ImageControl::instance = nullptr;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/image/exe.png"));

    //建立
    TLog::buildInstance();                   // 0
    Base::buildInstance();                   // 1
    SQLite::buildInstance();                 // 1
    DataActive::buildInstance();             // 2
    MediaPlayer::buildInstance();
    Setting::buildInstance();

    FileManagement::buildInstance();
    TaskCenter::buildInstance();
    OnLine::buildInstance();
    ImageControl::buildInstance();

    //获得单例指针
    SQLite *sql = SQLite::getInstance();
    Setting *seit = Setting::getInstance();
    DataActive *dataActive = DataActive::getInstance();
    MediaPlayer *mediaPlayer = MediaPlayer::getInstance();
    TaskCenter *center = TaskCenter::getInstance();
    OnLine *onLine = OnLine::getInstance();
    Base *base = Base::getInstance();
    FileManagement *fileMan = FileManagement::getInstance();
    ImageControl *imgCtr = ImageControl::getInstance();

    // 注册单例
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<Base>("MediaerAPI", 1, 0, "Base", base);
    qmlRegisterSingletonInstance<DataActive>("MediaerAPI", 1, 0, "DataActive", dataActive);
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
