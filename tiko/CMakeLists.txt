add_library(Tiko)
project(TikoLib VERSION 0.1 LANGUAGES CXX)

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
    TikoTextLine.qml
    TikoFrameless.qml
    TikoWidget.qml
    control/TikoBar.qml
    control/TikoButtonBorder.qml
    control/TikoButtonNormal.qml
    control/TikoButtonBase.qml
    control/TikoButtonCombox.qml
    control/TikoTextInput.qml
    control/TikoMenu.qml
    control/TikoMenuItem.qml
    control/TikoMenuSpeacer.qml
    control/TikoLittleMessage.qml
    control/TikoProgressBar.qml
    control/TikoProgressRing.qml
    control/TikoPopup.qml
    control/TikoPopupInput.qml
    control/TikoPopupMessage.qml
    control/TikoHSlider.qml
    control/TikoVSlider.qml
    control/TikoButtonSwitch.qml
    control/TikoButtonIcon.qml
    combox/TikoRightVessel.qml
    combox/TikoSelectColor.qml
    combox/TikoSelectFont.qml
    RESOURCES icon.qrc
    QML_FILES TikoImageAuto.qml
)
