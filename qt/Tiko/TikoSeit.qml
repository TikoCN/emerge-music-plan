pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI

QtObject {

    property bool isLightTheme: true

    property int subitemSpace: 6
    property int normalMargins: 10
    property int normalLineHeight: textNormalFont.pointSize * 1.3 + 2 * normalMargins

    // 颜色管理
    property color themeColor: Setting.themeColor
    property color transparentColor: Setting.transparentColor
    property color backdropColor: Setting.backdropColor

    // 歌词相关
    property color lrcNormalColor: Setting.lrcNormalColor
    property color lrcPlayingColor: Setting.lrcPlayingColor
    property color deskLrcColor: Setting.deskLrcColor

    // 字体颜色相关
    property color textBenchmarkColor: Setting.textBenchmarkColor
    onTextBenchmarkColorChanged: {
        textNormalColor = Setting.textBenchmarkColor
        let hsl = Qt.hsl(color)
        let ratio = isLightTheme ? 0.1 : 0.05
        hsl.lightness = Math.min(1.0, hsl.lightness + ratio)
        textNormalColor = Qt.color(hsl)
    }

    property color textNormalColor: "#333333"
    property color textTitleColor: "#1A1A1A"
    property color textSubtitleColor: "#444444"
    property color textInformationColor: "#666666"
    property color textAssistanceColor: "#888888"
    property color textDisabledColor: "#999999"
    // 按钮颜色相关
    property color buttonBenchmarkColor: Setting.buttonBenchmarkColor

    property color iconShowButtonColor: "#333333"
    property color iconHoverButtonColor: "#1A1A1A"
    property color iconNormalButtonColor: "#333333"
    property color iconPressedButtonColor: "#0D0D0D"
    property color iconDisabledButtonColor: "#999999"

    property color textShowButtonColor: "#333333"
    property color textHoverButtonColor: "#1A1A1A"
    property color textNormalButtonColor: "#333333"
    property color textPressedButtonColor: "#0D0D0D"
    property color textDisabledButtonColor: "#999999"

    property color backgroundShowButtonColor: "#F5F5F5"
    property color backgroundHoverButtonColor: "#ECECEC"
    property color backgroundNormalButtonColor: "#F5F5F5"
    property color backgroundPressedButtonColor: "#E0E0E0"
    property color backgroundDisabledButtonColor: "#F9F9F9"

    property color borderShowButtonColor: "#E5E5E5"
    property color borderHoverButtonColor: "#DCDCDC"
    property color borderNormalButtonColor: "#E5E5E5"
    property color borderPressedButtonColor: "#D0D0D0"
    property color borderDisabledButtonColor: "#F0F0F0"

    // 字体管理
    property font lrcFont: Setting.lrcFont
    property font deskLrcFont: Setting.deskLrcFont

    property font benchmarkFont: Setting.benchmarkFont
    property font normalFont: null
    property font titleFont: null
    property font subtitleFont: null
    property font informationFont: null
    property font assistanceFont: null
}
