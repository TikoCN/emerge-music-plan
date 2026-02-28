cmake_minimum_required(VERSION 3.20)
project(musicPlay VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# windeployqt.exe
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appmusicPlay
    main.cpp
        mediaplay/FrequencySpectrum.cpp
        mediaplay/FrequencySpectrum.h
)

qt_policy(SET QTP0004 NEW)
add_subdirectory(Tiko)
add_subdirectory(PlayView)

qt_add_qml_module(appmusicPlay
        URI musicPlay
        VERSION 1.0
        SOURCES
        baseclass/SortType.h
        Setting.h Setting.cpp
        OnLine.h OnLine.cpp
        FFmpeg.h FFmpeg.cpp

        sqlite/Sqlite.h sqlite/Sqlite.cpp
        sqlite/Append.h sqlite/Append.cpp
        sqlite/Update.h sqlite/Update.cpp
        sqlite/Get.h sqlite/Get.cpp
        sqlite/Core.h sqlite/Core.cpp
        sqlite/Delete.h sqlite/Delete.cpp


        baseclass/DataException.h
        baseclass/Album.h baseclass/Album.cpp
        baseclass/LrcData.h baseclass/LrcData.cpp
        baseclass/Music.h baseclass/Music.cpp
        baseclass/Playlist.h baseclass/Playlist.cpp
        baseclass/Artist.h baseclass/Artist.cpp
        baseclass/LiteralConstant.h baseclass/LiteralConstant.cpp
        baseclass/MediaData.h

        load/TaskCenter.h load/TaskCenter.cpp
        load/SelectMusicUrl.h load/SelectMusicUrl.cpp
        load/BuildMusicCore.h load/BuildMusicCore.cpp
        Tlog.h Tlog.cpp
        NameKey.h NameKey.cpp
        basetool/FileManagement.h basetool/FileManagement.cpp
        imageload/ImageControl.h imageload/ImageControl.cpp
        imageload/ImageProvider.h imageload/ImageProvider.cpp
        datacore/DataManager.h datacore/DataManager.cpp
        datacore/DataActive.h datacore/DataActive.cpp
        basetool/TypeConversion.h basetool/TypeConversion.cpp
        basetool/BaseTool.h basetool/BaseTool.cpp
        basetool/QmlActive.h basetool/QmlActive.cpp
        mediaplay/MediaplayData.h mediaplay/MediaplayData.cpp
        mediaplay/LrcDataControl.h mediaplay/LrcDataControl.cpp
        mediaplay/Mediaplayer.h mediaplay/Mediaplayer.cpp
        RESOURCES
        ui.qrc  # 资源文件（图片/字体等）
)

target_link_libraries(appmusicPlay PRIVATE TikoLib)
target_link_libraries(appmusicPlay PRIVATE PlayViewLib)

# link fftw3
find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE FFTW3::fftw3)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE unofficial::sqlite3::sqlite3)

#link FFmpeg
# 设置FFmpeg根目录（注意路径斜杠）
set(FFMPEG_ROOT "C:/code/ExLib/ffmpeg")
# 定义包含目录
set(FFMPEG_INCLUDE_DIRS "${FFMPEG_ROOT}/include")
# 定义库目录
set(FFMPEG_LIBRARY_DIRS "${FFMPEG_ROOT}/lib")

# 列出需要链接的FFmpeg核心库（根据实际使用的模块调整）
set(FFMPEG_LIBRARIES
    avcodec
    avformat
    avutil
    avfilter
    swresample
    swscale
)

# 将FFmpeg链接到目标可执行文件
target_include_directories(appmusicPlay PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_directories(appmusicPlay PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(appmusicPlay PRIVATE ${FFMPEG_LIBRARIES})

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appmusicPlay PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appmusicPlay
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
    OUTPUT_NAME "music"
)

target_link_libraries(appmusicPlay PRIVATE Qt6::Multimedia)
target_link_libraries(appmusicPlay PRIVATE Qt6::Quick)
target_link_libraries(appmusicPlay PRIVATE Qt6::Network)

get_target_property(PLAYVIEW_QML_PATH PlayViewLib IMPORTED_QML_MODULE_DIRECTORY)
get_target_property(TIKO_QML_MODULE_IMPORT_PATH TikoLib IMPORTED_QML_MODULE_DIRECTORY)
list(APPEND QML_IMPORT_PATH ${TIKO_QML_MODULE_IMPORT_PATH})

include(GNUInstallDirs)
install(TARGETS appmusicPlay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
