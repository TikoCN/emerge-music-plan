pragma Singleton
import QtQuick
import MyAPI

QtObject {
    property color themeColor: Setting.themeColor
    property color transparentColor: Setting.transparentColor
    property color backdropColor: Setting.backdropColor
    property double fontPixelSize: Setting.mainFont.family
    property string fontFamily: Setting.mainFont.pixelSize
}
