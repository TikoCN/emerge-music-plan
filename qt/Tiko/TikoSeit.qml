pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI

QtObject {

    property int subitemSpace: 6
    property int normalMargins: 10
    property int normalLineHeight: textNormalFont.pixelSize * 1.3 + 2 * normalMargins

    // 颜色管理
    property color themeColor: Setting.themeColor
    property color transparentColor: Setting.transparentColor
    property color backdropColor: Setting.backdropColor

    // 歌词相关
    property color lrcNormalColor: Setting.lrcNormalColor
    property color lrcPlayingColor: Setting.lrcPlayingColor
    property color deskLrcColor: Setting.deskLrcColor

    // 字体颜色相关
    property color textNormalColor: Setting.textNormalColor
    property color textTitleColor: Setting.textTitleColor
    property color textSubtitleColor: Setting.textSubtitleColor
    property color textInformationColor: Setting.textInformationColor
    property color textAssistanceColor: Setting.textAssistanceColor
    property color textDisabledColor: Setting.textDisabledColor

    // 按钮颜色相关
    property color buttonNormalColor: Setting.buttonNormalColor
    property color buttonHoverColor: Setting.buttonHoverColor
    property color buttonPressedColor: Setting.buttonPressedColor
    property color buttonDisabledColor: Setting.buttonDisabledColor

    // 字体管理
    property font lrcFont: Setting.lrcFont
    property font deskLrcFont: Setting.deskLrcFont
    property font textNormalFont: Setting.textNormalFont
    property font titleFont: Setting.titleFont
    property font subtitleFont: Setting.subtitleFont
    property font informationFont: Setting.informationFont
    property font assistanceFont: Setting.assistanceFont
}
