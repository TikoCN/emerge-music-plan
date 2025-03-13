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
    CoreLrcLine.qml
    CoreMusicLine.qml
    PlayerLrcTable.qml
    PlayerTable.qml
    view/PageMain.qml
    view/PageSeit.qml
    view/ToolDeskLrc.qml
    view/ToolEditMusic.qml
    view/ViewBottomBar.qml
    view/ViewLeftBar.qml
    view/ViewMain.qml
    view/ViewPlayingTable.qml
    view/mainStyle/MainStyle1.qml
    view/mainStyle/MainStyle2.qml
    RESOURCES ui.qrc
)
