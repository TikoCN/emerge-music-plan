#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <QObject>
#include <QProcess>

#include "Setting.h"
#include "load/TaskCenter.h"
#include "mediaplay/Mediaplayer.h"
#include "OnLine.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "basetool/BaseTool.h"
#include "imageload/ImageControl.h"
#include "imageload/ImageProvider.h"

SQLite *SQLite::instance = nullptr;
Setting *Setting::instance = nullptr;
TaskCenter *TaskCenter::instance = nullptr;
MediaPlayer *MediaPlayer::instance = nullptr;
DataActive *DataActive::instance = nullptr;
OnLine *OnLine::instance = nullptr;
TLog *TLog::instance = nullptr;
BaseTool *BaseTool::instance = nullptr;
ImageControl *ImageControl::instance = nullptr;

int main(int argc, char *argv[]) {
    const QGuiApplication app(argc, argv);

    FileManagement::makeAllDir();

    //建立
    TLog::buildInstance(); // 0
    TLog *tlog = TLog::getInstance();
    BaseTool::buildInstance(); // 1
    BaseTool *baseTool = BaseTool::getInstance();
    SQLite::buildInstance(tlog, baseTool); // 2
    SQLite *sql = SQLite::getInstance();

    DataActive::buildInstance(); // 2
    DataActive *dataActive = DataActive::getInstance();

    MediaPlayer::buildInstance(baseTool, dataActive, tlog, sql); // 3
    Setting::buildInstance(); // 3
    TaskCenter::buildInstance(); // 3
    OnLine::buildInstance(); // 3
    ImageControl::buildInstance(); // 3

    //获得单例指针
    const auto seit = Setting::getInstance();
    const auto mediaPlayer = MediaPlayer::getInstance();
    const auto center = TaskCenter::getInstance();
    const auto onLine = OnLine::getInstance();
    const auto imgCtr = ImageControl::getInstance();
    const auto imgPrd = new ImageProvider();

    qmlRegisterUncreatableType<Album>("MediaerAPI", 1, 0, "albumData","无法直接创建Album实例");
    qmlRegisterUncreatableType<Music>("MediaerAPI", 1, 0, "musicData","无法直接创建Music实例");
    qmlRegisterUncreatableType<PlayList>("MediaerAPI", 1, 0, "playListData","无法直接创建PlayList实例");
    qmlRegisterUncreatableType<Artist>("MediaerAPI", 1, 0, "artistData","无法直接创建Artist实例");
    // 注册单例
    qmlRegisterSingletonInstance<BaseTool>("MediaerAPI", 1, 0, "BaseTool", baseTool);
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<DataActive>("MediaerAPI", 1, 0, "DataActive", dataActive);
    qmlRegisterSingletonInstance<SQLite>("MediaerAPI", 1, 0, "SQLData", sql);
    qmlRegisterSingletonInstance<ImageControl>("MediaerAPI", 1, 0, "ImageControl", imgCtr);

    // 注册数据类
    qmlRegisterType<FileManagement>("DataType", 1, 0, "FileMan");
    qmlRegisterType<TypeConversion>("DataType", 1, 0, "TypeConversion");
    qmlRegisterType<QmlActive>("DataType", 1, 0, "QmlActive");

    QObject::connect(seit, &Setting::loadMusics, center, &TaskCenter::start);

    QObject::connect(mediaPlayer, &MediaPlayer::downLrc, onLine, &OnLine::downLrc);
    QObject::connect(mediaPlayer->getAudioOutput(), &QAudioOutput::volumeChanged, seit, [](const float volume) {
        Setting::setParameter(LiteralConstant::VOLUME, volume);
    });

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
    engine.addImageProvider("cover", imgPrd);
    engine.loadFromModule("PlayView", "Main");

    return QGuiApplication::exec();
}