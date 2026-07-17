#include <QAudioOutput>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QProcess>
#include <qsemaphore.h>
#include <QThread>

#include "Setting.h"
#include "load/TaskCenter.h"
#include "mediaplay/Mediaplayer.h"
#include "OnLine.h"
#include "datacore/DataActive.h"
#include "sqlite/Sqlite.h"
#include "basetool/BaseTool.h"
#include "imageload/ImageControl.h"
#include "imageload/ImageProvider.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    FileManagement::makeAllDir();

    SQLite *         sql         = &SQLite::getInstance();
    TLog *           tlog        = &TLog::getInstance();
    BaseTool *       baseTool    = &BaseTool::getInstance();
    DataActive *     dataActive  = &DataActive::getInstance();
    Setting *        seit        = &Setting::getInstance();
    MediaPlayer *    mediaPlayer = &MediaPlayer::getInstance();
    TaskCenter *     center      = &TaskCenter::getInstance();
    OnLine *         onLine      = &OnLine::getInstance();
    ImageControl *   imgCtr      = &ImageControl::getInstance();
    ImageProvider *  imgPrd      = new ImageProvider();

    qmlRegisterUncreatableType<Album>("MediaerAPI", 1, 0, "albumData", "无法直接创建Album实例");
    qmlRegisterUncreatableType<Music>("MediaerAPI", 1, 0, "musicData", "无法直接创建Music实例");
    qmlRegisterUncreatableType<PlayList>("MediaerAPI", 1, 0, "playListData", "无法直接创建PlayList实例");
    qmlRegisterUncreatableType<Artist>("MediaerAPI", 1, 0, "artistData", "无法直接创建Artist实例");
    qmlRegisterUncreatableType<LrcData>("MediaerAPI", 1, 0, "lrcData", "无法直接创建LrcData实例");

    qmlRegisterSingletonInstance<BaseTool>("MediaerAPI", 1, 0, "BaseTool", baseTool);
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<DataActive>("MediaerAPI", 1, 0, "DataActive", dataActive);
    qmlRegisterSingletonInstance<SQLite>("MediaerAPI", 1, 0, "SQLData", sql);
    qmlRegisterSingletonInstance<ImageControl>("MediaerAPI", 1, 0, "ImageControl", imgCtr);

    qmlRegisterType<FileManagement>("DataType", 1, 0, "FileMan");
    qmlRegisterType<TypeConversion>("DataType", 1, 0, "TypeConversion");
    qmlRegisterType<QmlActive>("DataType", 1, 0, "QmlActive");

    QObject::connect(seit, &Setting::loadMusics, center, &TaskCenter::start);

    QObject::connect(mediaPlayer, &MediaPlayer::downLrc, onLine, &OnLine::downLrc);
    QObject::connect(mediaPlayer->getAudioOutput(), &QAudioOutput::volumeChanged, seit, [](const float volume) {
        Setting::setParameter(LiteralConstant::VOLUME, volume);
    });

    QObject::connect(onLine, &OnLine::lrcDowned, mediaPlayer, &MediaPlayer::loadLrcList);

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

    QObject::connect(&app, &QGuiApplication::aboutToQuit, []() {
        MediaPlayer::getInstance().getPlayer()->stop();
        QThread::msleep(100);
    });

    const int ret = QGuiApplication::exec();

    return ret;
}
