#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QObject>
#include <QProcess>

#include "setting.h"
#include "load/taskcenter.h"
#include "mediaplay/mediaplayer.h"
#include "online.h"
#include "datacore/dataactive.h"
#include "sqlite/sqlite.h"
#include "basetool/basetool.h"
#include "imageload/imagecontrol.h"
#include "imageload/imageprovider.h"

SQLite *SQLite::instance = nullptr;
Setting* Setting::instance = nullptr;
TaskCenter* TaskCenter::instance = nullptr;
MediaPlayer* MediaPlayer::instance = nullptr;
DataActive* DataActive::instance = nullptr;
OnLine* OnLine::instance = nullptr;
TLog *TLog::instance = nullptr;
BaseTool *BaseTool::instance = nullptr;
ImageControl *ImageControl::instance = nullptr;

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    FileManagement::makeAllDir();

    //建立
    TLog::buildInstance();                                      // 0
    TLog *tlog = TLog::getInstance();
    SQLite::buildInstance(tlog);                                // 1
    BaseTool::buildInstance();                                  // 1
    BaseTool *baseTool = BaseTool::getInstance();

    DataActive::buildInstance();                                 // 2
    DataActive *dataActive = DataActive::getInstance();

    MediaPlayer::buildInstance(baseTool, dataActive, tlog);      // 3
    Setting::buildInstance();                                    // 3
    TaskCenter::buildInstance();                                 // 3
    OnLine::buildInstance();                                     // 3
    ImageControl::buildInstance();                               // 3

    //获得单例指针
    SQLite *sql = SQLite::getInstance();
    Setting *seit = Setting::getInstance();
    MediaPlayer *mediaPlayer = MediaPlayer::getInstance();
    const TaskCenter *center = TaskCenter::getInstance();
    OnLine *onLine = OnLine::getInstance();
    ImageControl *imgCtr = ImageControl::getInstance();

    // 注册单例
    qmlRegisterSingletonInstance<BaseTool>("MediaerAPI", 1, 0, "BaseTool", baseTool);  // 0
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<DataActive>("MediaerAPI", 1, 0, "DataActive", dataActive);
    qmlRegisterSingletonInstance<SQLite>("MediaerAPI", 1, 0, "SQLData", sql);
    qmlRegisterSingletonInstance<ImageControl>("MediaerAPI", 1, 0, "ImageControl", imgCtr);

    // 注册数据类
    qmlRegisterType<LrcData>("DataType", 1, 0, "LrcData");
    qmlRegisterType<FileManagement>("DataType", 1, 0, "FileMan");
    qmlRegisterType<TypeConversion>("DataType", 1, 0, "TypeConversion");
    qmlRegisterType<QmlActive>("DataType", 1, 0, "QmlActive");

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
    engine.addImageProvider("cover", new ImageProvider);
    engine.loadFromModule("PlayView", "Main");

    return app.exec();
}
