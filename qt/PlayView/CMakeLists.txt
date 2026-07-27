cmake_minimum_required(VERSION 3.16)

project(PlayViewLib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.5)

add_library(PlayViewLib STATIC)

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
        AutoCoverImage.qml
        LoadBase.qml


        MainPage/PageSeit.qml
        MainPage/PageBase.qml
        MainPage/PageLibrary.qml
        MainPage/PagePlayList.qml
        MainPage/PageArtist.qml
        MainPage/PageAlbum.qml
        MainPage/PageRecommend.qml

        PageSeit/FontSeit.qml
        PageSeit/SourceSeit.qml
        PageSeit/ParameterSeit.qml
        PageSeit/UpdateDataSeit.qml
        PageSeit/SwitchSeit.qml
        PageSeit/ColorSeit.qml

        View/ViewLeftBar.qml
        View/ViewBottomBar.qml
        View/ViewMain.qml
        View/ViewPlayingList.qml

        Background/BackImageMove.qml
        Background/BackImage.qml
        Background/BackWater.qml

        LineCell/CoreMusicLine.qml

        Menu/MenuAlbum.qml
        Menu/MenuPlayList.qml
        Menu/MenuArtist.qml
        Menu/MenuMusic.qml
        Menu/MenuPlayListSort.qml
        Menu/MenuMusicPlayList.qml

        GeneralControls/WindowMin.qml
        GeneralControls/PlayState.qml
        GeneralControls/WindowClose.qml
        GeneralControls/WindowMax.qml
        GeneralControls/PlayUp.qml
        GeneralControls/PlayDown.qml
        GeneralControls/PositionControl.qml

        PageLibrary/LibraryPage.qml
        PageLibrary/LibraryMusic.qml
        PageLibrary/LibraryArtist.qml
        PageLibrary/LibraryAlbum.qml

        PageRecommend/RecomGridButtonBase.qml
        PageRecommend/RecomMusicNew.qml
        PageRecommend/RecomMusicPlay.qml
        PageRecommend/RecomGridButtonMusic.qml
        PageRecommend/RecomGridButtonArtist.qml
        PageRecommend/RecomGridButtonAlbum.qml

        ToolEditMusicPage/ToolEditMusic.qml
        ToolEditMusicPage/EditPageMusic.qml
        ToolEditMusicPage/EditPageLrc.qml
        ToolEditMusicPage/ToolDeskLrc.qml

        AudioVisualizationRectangle/AudioVsualizationRect.qml
        AudioVisualizationRectangle/AudioVsualizationCircle.qml

        PlayMusicShow/PageMusicPlay.qml
        PlayMusicShow/PlayStyle1.qml
        PlayMusicShow/PlayStyle2.qml

        ButtonCell/ButtonBase.qml
        ButtonCell/ButtonMusic.qml
        ButtonCell/ButtonArtist.qml
        ButtonCell/ButtonAlbum.qml

        LineList/ListViewMusic.qml
        LineList/ListViewLrc.qml

        LineLrc/CoreLrcLine.qml
        LineLrc/LrcLineNormal.qml
        LineLrc/LrcLineLoad.qml
        LineLrc/LrcLineBase.qml
        LineLrc/LrcLineDesk.qml

        GridButton/GridButtonMusic.qml
        GridButton/GridButtonArtist.qml
        GridButton/GridButtonAlbum.qml
        QML_FILES PlayMusicShow/PlayStyle3.qml
)

target_link_libraries(PlayViewLib PRIVATE Qt6::Quick)

install(TARGETS PlayViewLib
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
