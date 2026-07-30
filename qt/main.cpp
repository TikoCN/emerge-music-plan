#include <QAudioOutput>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QObject>
#include <QProcess>
#include <QThread>

#include "Setting.h"
#include "load/TaskCenter.h"
#include "mediaplay/Mediaplayer.h"
#include "OnLine.h"
#include "datacore/DataActive.h"
#include "library/MusicLibrary.h"
#include "library/ArtistLibrary.h"
#include "library/AlbumLibrary.h"
#include "library/PlaylistLibrary.h"
#include "sqlite/Sqlite.h"
#include "basetool/BaseTool.h"
#include "imageload/ImageControl.h"
#include "imageload/ImageProvider.h"
#include "model/AlbumModel.h"
#include "model/ArtistModel.h"
#include "model/PlaylistModel.h"
#include "model/MusicModel.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    FileManagement::makeAllDir();

    SQLite *       sql         = &SQLite::getInstance();
    TLog *         tlog        = &TLog::getInstance();
    BaseTool *     baseTool    = &BaseTool::getInstance();
    DataActive *   dataActive  = &DataActive::getInstance();
    Setting *      seit        = &Setting::getInstance();
    MediaPlayer *  mediaPlayer = &MediaPlayer::getInstance();
    TaskCenter *   center      = &TaskCenter::getInstance();
    OnLine *       onLine      = &OnLine::getInstance();
    ImageControl * imgCtr      = &ImageControl::getInstance();
    ImageProvider *imgPrd      = new ImageProvider();

    qmlRegisterUncreatableType<Album>("MediaerAPI", 1, 0, "albumData", "无法直接创建Album实例");
    qmlRegisterUncreatableType<Music>("MediaerAPI", 1, 0, "musicData", "无法直接创建Music实例");
    qmlRegisterUncreatableType<Playlist>("MediaerAPI", 1, 0, "playListData", "无法直接创建Playlist实例");
    qmlRegisterUncreatableType<Artist>("MediaerAPI", 1, 0, "artistData", "无法直接创建Artist实例");
    qmlRegisterUncreatableType<LrcData>("MediaerAPI", 1, 0, "lrcData", "无法直接创建LrcData实例");

    qmlRegisterSingletonInstance<BaseTool>("MediaerAPI", 1, 0, "BaseTool", baseTool);
    qmlRegisterSingletonInstance<Setting>("MediaerAPI", 1, 0, "Setting", seit);
    qmlRegisterSingletonInstance<TaskCenter>("MediaerAPI", 1, 0, "TaskCenter", center);
    qmlRegisterSingletonInstance<MediaPlayer>("MediaerAPI", 1, 0, "MediaPlayer", mediaPlayer);
    qmlRegisterSingletonInstance<OnLine>("MediaerAPI", 1, 0, "OnLine", onLine);
    qmlRegisterSingletonInstance<ImageControl>("MediaerAPI", 1, 0, "ImageControl", imgCtr);
    MusicLibrary *   musicLibrary    = &MusicLibrary::getInstance();
    ArtistLibrary *  artistLibrary   = &ArtistLibrary::getInstance();
    AlbumLibrary *   albumLibrary    = &AlbumLibrary::getInstance();
    PlaylistLibrary *playListLibrary = &PlaylistLibrary::getInstance();
    qmlRegisterSingletonInstance<MusicLibrary>("MediaerAPI", 1, 0, "MusicLibrary", musicLibrary);
    qmlRegisterSingletonInstance<ArtistLibrary>("MediaerAPI", 1, 0, "ArtistLibrary", artistLibrary);
    qmlRegisterSingletonInstance<AlbumLibrary>("MediaerAPI", 1, 0, "AlbumLibrary", albumLibrary);
    qmlRegisterSingletonInstance<PlaylistLibrary>("MediaerAPI", 1, 0, "PlaylistLibrary", playListLibrary);

    qmlRegisterType<MusicModel>("MediaerAPI", 1, 0, "MusicModel");
    qmlRegisterType<AlbumModel>("MediaerAPI", 1, 0, "AlbumModel");
    qmlRegisterType<ArtistModel>("MediaerAPI", 1, 0, "ArtistModel");
    qmlRegisterType<PlaylistModel>("MediaerAPI", 1, 0, "PlaylistModel");

    qmlRegisterType<FileManagement>("DataType", 1, 0, "FileMan");
    qmlRegisterType<TypeConversion>("DataType", 1, 0, "TypeConversion");
    qmlRegisterType<QmlActive>("DataType", 1, 0, "QmlActive");

    QObject::connect(seit, &Setting::loadMusics, center, &TaskCenter::startLoadFile);

    QObject::connect(mediaPlayer, &MediaPlayer::downLrc, onLine, &OnLine::downLrc);
    QObject::connect(mediaPlayer->getAudioOutput(), &QAudioOutput::volumeChanged, seit, [](const float volume) {
        Setting::setParameter(LiteralConstant::VOLUME, volume);
    });

    QObject::connect(onLine, &OnLine::lrcDowned, mediaPlayer, &MediaPlayer::loadLrcList);

    // 启动同步序列：先清理无效数据，再加载音乐
    center->startBootSequence();

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
