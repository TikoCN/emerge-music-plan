cmake_minimum_required(VERSION 3.16)
add_library(Tiko)
project(TikoLib VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Quick)

set(CMAKE_AUTORCC true)
set_source_files_properties(TikoSeit.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_policy(SET QTP0004 NEW)
qt_add_qml_module(TikoLib
    URI Tiko
    VERSION 1.0
    QML_FILES
    TikoSeit.qml
    TikoFrameless.qml
    TikoWidget.qml
    bar/TikoBarV.qml
    button/TikoButtonBorder.qml
    button/TikoButtonNormal.qml
    button/TikoButtonBase.qml
    button/TikoButtonCombox.qml
    button/TikoButtonSwitch.qml
    button/TikoButtonIcon.qml
    text/TikoTextInput.qml
    text/TikoTextLine.qml
    menu/TikoMenu.qml
    menu/TikoMenuItem.qml
    menu/TikoMenuSpeacer.qml
    message/TikoMessageLittle.qml
    progress/TikoProgressBar.qml
    progress/TikoProgressRing.qml
    popup/TikoPopup.qml
    popup/TikoPopupInput.qml
    popup/TikoPopupMessage.qml
    slider/TikoSliderH.qml
    slider/TikoSliderV.qml
    vessel/TikoRightVessel.qml
    select/TikoSelectColor.qml
    select/TikoSelectFont.qml
    RESOURCES icon.qrc
    QML_FILES TikoImageAuto.qml
)
