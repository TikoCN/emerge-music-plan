cmake_minimum_required(VERSION 3.16)

project(musicPlay VERSION 0.1 LANGUAGES CXX)
# 设置 vcpkg 工具链路径
# set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Multimedia)
find_package(Qt6 REQUIRED COMPONENTS Network)

qt_standard_project_setup(REQUIRES 6.5)

qt_add_executable(appmusicPlay
    main.cpp
)

qt_add_qml_module(appmusicPlay
    URI musicPlay
    VERSION 1.0
    QML_FILES
    Main.qml
    QML_FILES CoreMusicLine.qml
    QML_FILES PlayerTable.qml
    QML_FILES PageSeit.qml
    QML_FILES ViewLeftBar.qml
    QML_FILES ViewRightConter.qml
    QML_FILES ViewBottomBar.qml
    QML_FILES MyBarButton.qml
    QML_FILES MyAutoText.qml
    QML_FILES MyBar.qml
    QML_FILES PageMain.qml
    QML_FILES CoreLrcLine.qml
    QML_FILES PlayerLrcTable.qml
    QML_FILES ViewPlayingTable.qml
    QML_FILES MyBorderButton.qml
    QML_FILES PopupMessage.qml
    QML_FILES ToolEditMusic.qml
    QML_FILES MyFrameless.qml
    QML_FILES MyUiButton.qml
    QML_FILES MyInputText.qml
    QML_FILES PlayerSpectrum.qml
    QML_FILES MyRightVessel.qml
    QML_FILES SelectColor.qml
    QML_FILES SelectFont.qml
    QML_FILES MyPopup.qml
    QML_FILES MySlider.qml
    QML_FILES MyMenu.qml
    QML_FILES MyMenuItem.qml
    QML_FILES MyCheckButton.qml
    QML_FILES PopupInput.qml
    QML_FILES ToolDeskLrc.qml
    SOURCES mediaplayer.h mediaplayer.cpp
    SOURCES setting.h setting.cpp
    SOURCES hosttime.h hosttime.cpp
    SOURCES table.h table.cpp
    SOURCES extralibrary.h extralibrary.cpp
    SOURCES imageprovider.h imageprovider.cpp
    SOURCES toolhelper.h toolhelper.cpp
    SOURCES popupdata.h popupdata.cpp
    SOURCES lrcdata.h lrcdata.cpp
    SOURCES music.h music.cpp
    RESOURCES ui.qrc
)

# link fftw3
find_package(FFTW3 CONFIG REQUIRED)
target_link_libraries(appmusicPlay PRIVATE FFTW3::fftw3)

# link TagLib
find_package(TagLib REQUIRED)
target_link_libraries(appmusicPlay PRIVATE TagLib::TagLib)

# link FFMPEG
find_package(FFMPEG REQUIRED)
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
