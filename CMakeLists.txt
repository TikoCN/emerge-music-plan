cmake_minimum_required(VERSION 3.16)
project(musicPlay VERSION 0.1 LANGUAGES CXX)
set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appmusicPlay
    main.cpp
)

qt_policy(SET QTP0004 NEW)
add_subdirectory(tiko)
add_subdirectory(ups)

qt_add_qml_module(appmusicPlay
    URI musicPlay
    VERSION 1.0
    SOURCES
    mediaplayer.h mediaplayer.cpp
    setting.h setting.cpp
    imageprovider.h imageprovider.cpp
    online.h online.cpp
    ffmpeg.h ffmpeg.cpp
    base.h base.cpp
    musiccore.h musiccore.cpp
    sqlite/sqlite.h sqlite/sqlite.cpp sqlite/update.cpp sqlite/append.cpp
    base/keyvaluepair.h base/keyvaluepair.cpp
    base/album.h base/album.cpp
    base/lrcdata.h base/lrcdata.cpp
    base/music.h base/music.cpp
    base/table.h base/table.cpp
    base/artist.h base/artist.cpp
    load/taskcenter.h load/taskcenter.cpp
    load/selectmusicurl.h load/selectmusicurl.cpp
    load/buildmusiccore.h load/buildmusiccore.cpp
    base/mediadata.h
    SOURCES sqlite/get.cpp
    SOURCES sqlite/appned.h
    SOURCES sqlite/get.h
    SOURCES sqlite/update.h
    SOURCES sqlite/core.h sqlite/core.cpp
)

# link fftw3
find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE FFTW3::fftw3)

# link TagLib
find_package(TagLib REQUIRED)
target_link_libraries(appmusicPlay PRIVATE TagLib::TagLib)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE unofficial::sqlite3::sqlite3)

#link FFmpeg
# 设置FFmpeg根目录（注意路径斜杠）
set(FFMPEG_ROOT "C:/Users/18215/Documents/ExLib/ffmpeg")
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


include(GNUInstallDirs)
install(TARGETS appmusicPlay
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
