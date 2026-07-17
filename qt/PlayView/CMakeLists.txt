cmake_minimum_required(VERSION 3.16)

# 规范：先定义项目信息，再创建库目标
project(PlayViewLib VERSION 0.1 LANGUAGES CXX)

# 设置基础编译配置
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt 依赖
find_package(Qt6 REQUIRED COMPONENTS Quick)
qt_standard_project_setup(REQUIRES 6.5)

# 创建静态库目标
add_library(PlayViewLib STATIC)

# 标记单例 QML 文件
set_source_files_properties(CoreData.qml
        PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

# 允许 QML 模块关联到静态库
qt_policy(SET QTP0004 NEW)
# 添加 QML 模块
qt_add_qml_module(PlayViewLib
        URI PlayView                  # 模块唯一标识，主项目通过这个导入
        VERSION 1.0                   # 版本号
        QML_FILES
        # 其他 QML 文件
        Main.qml
        CoreData.qml
        AutoCoverImage.qml
        LoadBase.qml
        # 播放器页面相关
        player/ShowPagePlayList.qml
        player/ShowPageAlbum.qml
        player/ShowPageArtist.qml
        player/ShowPageBase.qml
        # 视图相关
        view/ViewBottomBar.qml
        view/ViewLeftBar.qml
        view/ViewMain.qml
        view/ViewPlayingList.qml
        # 播放样式相关
        playMusicShow/PageMusicPlay.qml
        playMusicShow/PlayStyle1.qml
        playMusicShow/PlayStyle2.qml
        # 菜单相关
        menu/MenuPlayListSort.qml
        menu/MenuMusic.qml
        menu/MenuPlayList.qml
        menu/MenuAlbum.qml
        menu/MenuArtist.qml
        menu/MenuMusicPlayList.qml
        # 编辑页面相关
        toolEditMusicPage/EditPageMusic.qml
        toolEditMusicPage/EditPageLrc.qml
        toolEditMusicPage/ToolEditMusic.qml
        toolEditMusicPage/ToolDeskLrc.qml
        # 推荐相关
        recommend/RecomGridButtonAlbum.qml
        recommend/RecomGridButtonArtist.qml
        recommend/RecomGridButtonMusic.qml
        recommend/RecomMusicNew.qml
        recommend/RecomMusicPlay.qml
        recommend/RecomGridButtonBase.qml
        recommend/PageRecommend.qml
        # 歌词行相关
        lrcLine/LrcLineNormal.qml
        lrcLine/LrcLineLoad.qml
        lrcLine/CoreLrcLine.qml
        lrcLine/LrcLineDesk.qml
        lrcLine/LrcLineBase.qml
        # 输入弹窗相关
        inputPopup/InputArtistName.qml
        inputPopup/InputAlbumName.qml
        inputPopup/InputPlayListName.qml
        # 核心组件相关
        lineCell/CoreMusicLine.qml
        lineList/ListViewMusic.qml
        lineList/ListViewLrc.qml

        seitPage/UpdateDataSeit.qml
        seitPage/SourceSeit.qml
        seitPage/ColorSeit.qml
        seitPage/FontSeit.qml
        seitPage/ParameterSeit.qml
        seitPage/SwitchSeit.qml
        seitPage/PageSeit.qml

        buttonCell/CoreButtonBase.qml
        buttonCell/CoreButtonMusic.qml
        buttonCell/CoreButtonAlbum.qml
        buttonCell/CoreButtonArtist.qml

        buttonGrid/GridButtonBase.qml
        buttonGrid/GridButtonArtist.qml
        buttonGrid/GridButtonAlbum.qml
        buttonGrid/GridButtonMusic.qml

        libraryPage/PageButtonBase.qml
        libraryPage/PageMusic.qml
        libraryPage/PageArtist.qml
        libraryPage/PageAlbum.qml
        libraryPage/PageLibrary.qml

        back/BackImage.qml
        back/BackWater.qml
        back/BackImageMove.qml

        QML_FILES AudioVisualizationRectangle/AudioVsualizationRect.qml
        QML_FILES AudioVisualizationRectangle/AudioVsualizationCircle.qml

        QML_FILES GeneralControls/PositionControl.qml
        QML_FILES GeneralControls/WindowClose.qml
        QML_FILES GeneralControls/WindowMax.qml
        QML_FILES GeneralControls/WindowMin.qml
        QML_FILES GeneralControls/PlayUp.qml
        QML_FILES GeneralControls/PlayDown.qml
        QML_FILES GeneralControls/PlayState.qml
)

# 链接 QtQuick 依赖
target_link_libraries(PlayViewLib PRIVATE Qt6::Quick)

install(TARGETS PlayViewLib
        BUNDLE DESTINATION .
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
