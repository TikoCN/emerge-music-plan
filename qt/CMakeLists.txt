cmake_minimum_required(VERSION 3.20)
project(musicPlay VERSION 0.1 LANGUAGES CXX)

if(MSVC)
    add_compile_options(/MP)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# windeployqt.exe
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 16)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appmusicPlay
    main.cpp
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
        sqlite/Core.h sqlite/Core.cpp
        sqlite/MusicRepository.h sqlite/MusicRepository.cpp
        sqlite/AlbumRepository.h sqlite/AlbumRepository.cpp
        sqlite/ArtistRepository.h sqlite/ArtistRepository.cpp
        sqlite/PlayListRepository.h sqlite/PlayListRepository.cpp

        baseclass/DataException.h
        baseclass/Album.h baseclass/Album.cpp
        baseclass/LrcData.h baseclass/LrcData.cpp
        baseclass/Music.h baseclass/Music.cpp
        baseclass/Playlist.h baseclass/Playlist.cpp
        baseclass/Artist.h baseclass/Artist.cpp
        baseclass/LiteralConstant.h baseclass/LiteralConstant.cpp
        baseclass/MediaData.h
        baseclass/LruCache.h

        load/TaskCenter.h load/TaskCenter.cpp
        load/FileScanner.h load/FileScanner.cpp
        load/MediaParser.h load/MediaParser.cpp
        load/FileExistCheckTask.h load/FileExistCheckTask.cpp
        Tlog.h Tlog.cpp
        NameKey.h NameKey.cpp
        basetool/FileManagement.h basetool/FileManagement.cpp
        imageload/ImageControl.h imageload/ImageControl.cpp
        imageload/ImageProvider.h imageload/ImageProvider.cpp
        datacore/DataManager.h datacore/DataManager.cpp
        datacore/DataActive.h datacore/DataActive.cpp
        datacore/DataLoader.h
        library/MusicLibrary.h library/MusicLibrary.cpp
        library/ArtistLibrary.h library/ArtistLibrary.cpp
        library/AlbumLibrary.h library/AlbumLibrary.cpp
        library/PlayListLibrary.h library/PlayListLibrary.cpp
        model/MusicModel.h model/MusicModel.cpp
        model/AlbumModel.h model/AlbumModel.cpp
        model/ArtistModel.h model/ArtistModel.cpp
        model/PlaylistModel.h model/PlaylistModel.cpp
        basetool/TypeConversion.h basetool/TypeConversion.cpp
        basetool/BaseTool.h basetool/BaseTool.cpp
        basetool/QmlActive.h basetool/QmlActive.cpp
        mediaplay/MediaplayData.h mediaplay/MediaplayData.cpp
        mediaplay/LrcDataControl.h mediaplay/LrcDataControl.cpp
        mediaplay/Mediaplayer.h mediaplay/Mediaplayer.cpp
        mediaplay/FrequencySpectrum.cpp
        mediaplay/FrequencySpectrum.h

        RESOURCES
        ui.qrc
        SOURCES macro/NotifyProperty.h
        SOURCES macro/ConstantProperty.h
        SOURCES macro/SettingAttribute.h  # 资源文件（图片/字体等）
)

target_link_libraries(appmusicPlay PRIVATE TikoLib)
target_link_libraries(appmusicPlay PRIVATE PlayViewLib)

# link fftw3
find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE FFTW3::fftw3)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE unofficial::sqlite3::sqlite3)

#link FFmpeg
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED
        libavcodec
        libavformat
        libavutil
        libswscale
        libswresample
)

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
list(APPEND QML_IMPORT_PATH ${PLAYVIEW_QML_PATH})

include(GNUInstallDirs)
install(TARGETS appmusicPlay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# 部署
find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${CMAKE_PREFIX_PATH}/bin")
if(WINDEPLOYQT_EXECUTABLE)
    add_custom_command(TARGET appmusicPlay POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}"
            --no-compiler-runtime
            --no-system-d3d-compiler
            --qmldir "${CMAKE_SOURCE_DIR}"
            "$<TARGET_FILE:appmusicPlay>"
            COMMENT "Deploying Qt and QML libraries to Debug directory..."
            WORKING_DIRECTORY "$<TARGET_FILE_DIR:appmusicPlay>"
    )
endif()