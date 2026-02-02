pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI

QtObject {

    property int normalMargins: 10
    property int fontHeight: fontPixelSize * 1.3
    property int fontWidth: fontPixelSize
    property int normalLineHeight: fontHeight + 2 * normalMargins

    property font mainFont: Setting.mainFont
    property font deskFont: Setting.deskFont
    property font mainLrcFont: Setting.mainLrcFont
    property font deskLrcFont: Setting.deskLrcFont
    property font textNormlFont: Setting.textNormalFont
    property font titleFont: Setting.titleFont
    property font assistanceFont: Setting.assistanceFont


    property color themeColor: Setting.themeColor
    property color transparentColor: Setting.transparentColor
    property color backdropColor: Setting.backdropColor
    property color deskLrcColor: Setting.deskLrcColor

    // 按钮相关颜色
    property color textNormalColor: Setting.textNormalColor
    property color buttonNormalColor: Setting.buttonNormalColor
    property color buttonHoverColor: Setting.buttonHoverColor
    property color buttonPressedColor: Setting.buttonPressedColor
}
