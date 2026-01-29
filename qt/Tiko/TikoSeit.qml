pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI

QtObject {
    property color themeColor: Setting.themeColor
    property color transparentColor: Setting.transparentColor
    property color backdropColor: Setting.backdropColor
    property double fontPixelSize: Setting.mainFont.pixelSize
    property int normalMargins: 10
    property int fontHeight: fontPixelSize * 1.3
    property int fontWidth: fontPixelSize
    property int normalLineHeight: fontHeight + 2 * normalMargins
    property string fontFamily: Setting.mainFont.family
    property bool isShowFramChange: true
}
