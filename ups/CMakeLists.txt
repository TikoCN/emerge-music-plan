cmake_minimum_required(VERSION 3.16)
add_library(Ups)
project(UpsLib VERSION 0.1 LANGUAGES CXX)

qt_standard_project_setup(REQUIRES 6.5)
find_package(Qt6 REQUIRED COMPONENTS Quick)

set(CMAKE_AUTORCC true)
set_source_files_properties(CoreData.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(UpsLib
    URI Ups
    VERSION 1.0
    QML_FILES
    Main.qml
    CoreData.qml
    CoreMusicLine.qml
    player/PlayerLrcTable.qml
    player/PlayerTable.qml
    ToolDeskLrc.qml
    view/ViewBottomBar.qml
    view/ViewLeftBar.qml
    view/ViewMain.qml
    view/ViewPlayingTable.qml
    playMusicShow/PlayStyle1.qml
    playMusicShow/PlayStyle2.qml
    playMusicShow/PageMusicPlay.qml
    menu/MenuTableSort.qml
    menu/MenuMusic.qml
    menu/MenuTable.qml
    toolEditMusicPage/EditPageMusic.qml
    toolEditMusicPage/EditPageLrc.qml
    toolEditMusicPage/ToolEditMusic.qml
    centerPage/PageArtist.qml
    centerPage/PageAlumb.qml
    centerPage/PageSeit.qml
    lrcLine/DrawLrcLine.qml
    lrcLine/DrawLoadLine.qml
    lrcLine/CoreLrcLine.qml
    centerPage/PageMain.qml
    CoreAlumbButton.qml
    player/PlayerAlumb.qml
    player/PlayerArtist.qml
    CoreArtistButton.qml
    lrcLine/DrawLrcFixHeight.qml
    QML_FILES core/CoreMusicButton.qml
    QML_FILES menu/MenuAlumb.qml
    QML_FILES menu/MenuArtist.qml
    RESOURCES ui.qrc
)

