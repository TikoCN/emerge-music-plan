cmake_minimum_required(VERSION 3.16)
add_library(PlayView)
project(PlayViewLib VERSION 0.1 LANGUAGES CXX)

qt_standard_project_setup(REQUIRES 6.5)
find_package(Qt6 REQUIRED COMPONENTS Quick)

set(CMAKE_AUTORCC true)
set_source_files_properties(CoreData.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(PlayViewLib
    URI PlayView
    VERSION 1.0
    QML_FILES
    Main.qml
    CoreData.qml
    ToolDeskLrc.qml
    player/ShowPageLrcList.qml
    player/ShowPagePlayList.qml
    player/ShowPageAlbum.qml
    player/ShowPageArtist.qml
    view/ViewBottomBar.qml
    view/ViewLeftBar.qml
    view/ViewMain.qml
    view/ViewPlayingList.qml
    playMusicShow/PlayStyle1.qml
    playMusicShow/PlayStyle2.qml
    menu/MenuPlayListSort.qml
    menu/MenuMusic.qml
<<<<<<< Updated upstream:PlayView/CMakeLists.txt
    menu/MenuTable.qml
=======
    menu/MenuPlayList.qml
    menu/MenuAlbum.qml
    menu/MenuArtist.qml
    menu/MenuMusicPlayList.qml
>>>>>>> Stashed changes:qt/PlayView/CMakeLists.txt
    toolEditMusicPage/EditPageMusic.qml
    toolEditMusicPage/EditPageLrc.qml
    toolEditMusicPage/ToolEditMusic.qml
    centerPage/PageArtist.qml
    centerPage/PageAlbum.qml
    centerPage/PageSeit.qml
    centerPage/PageMusicPlay.qml
    centerPage/PageRecommend.qml
    recommend/RecomAlbum.qml
    recommend/RecomArtist.qml
    recommend/RecomMusic.qml
    recommend/RecomMusicNew.qml
    recommend/RecomMusicPlay.qml
    lrcLine/DrawLrcLine.qml
    lrcLine/DrawLoadLine.qml
    lrcLine/CoreLrcLine.qml
    lrcLine/DrawLrcFixHeight.qml
    core/CoreMusicButton.qml
    core/CoreAlbumButton.qml
    core/CoreArtistButton.qml
    core/CoreMusicLine.qml
<<<<<<< Updated upstream:PlayView/CMakeLists.txt
    menu/MenuAlbum.qml
    menu/MenuArtist.qml
    RESOURCES ui.qrc
    QML_FILES menu/MenuMusicTable.qml
=======
    inputPopup/InputArtistName.qml
    inputPopup/InputAlbumName.qml
    inputPopup/InputPlayListName.qml
    RESOURCES ui.qrc
    QML_FILES AutoCoverImage.qml
>>>>>>> Stashed changes:qt/PlayView/CMakeLists.txt
)

