add_library(Widget)
project(WidgetLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTORCC true)
set_source_files_properties(CoreData.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(WidgetLib
    URI Widget
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
    RESOURCES ui.qrc
)
