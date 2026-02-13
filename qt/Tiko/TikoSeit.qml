pragma Singleton
import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI

QtObject {
    // 主题管理
    property bool isLightTheme: Setting.isLightTheme
    property color themeColor: Setting.themeColor
    onThemeColorChanged: buildTheme()
    onIsLightThemeChanged: buildTheme()

    // 字体颜色相关
    property TikoTheme theme: TikoTheme{}

    // 字体管理
    property font benchmarkFont: Setting.benchmarkFont
    onBenchmarkFontChanged: theme.textTheme.setFont(benchmarkFont)

    function buildTheme(){
        theme.setDynamicColor(themeColor, isLightTheme)
        theme.textTheme.setColor(isLightTheme)
        theme.baseTheme.setColor(themeColor, isLightTheme)
    }

    // 基于WCAG相对亮度计算
    function isLightColorByLuminance(color) {
        if (!color) return false

        // 计算相对亮度（WCAG公式）
        function getLuminance(color) {
            let r = color.r <= 0.03928 ? color.r / 12.92 : Math.pow((color.r + 0.055) / 1.055, 2.4)
            let g = color.g <= 0.03928 ? color.g / 12.92 : Math.pow((color.g + 0.055) / 1.055, 2.4)
            let b = color.b <= 0.03928 ? color.b / 12.92 : Math.pow((color.b + 0.055) / 1.055, 2.4)
            return 0.2126 * r + 0.7152 * g + 0.0722 * b
        }

        let luminance = getLuminance(color)

        // 相对亮度>0.5认为是亮色
        return luminance > 0.5
    }

    property int subitemSpace: 6
    property int normalMargins: 10
    property int emphasizeMargins: 20
    property int normalLineHeight: theme.textTheme.normalFont.pointSize * 1.3 + 2 * normalMargins

    // 歌词相关
    property color lrcNormalColor: Setting.lrcNormalColor
    property color lrcPlayingColor: Setting.lrcPlayingColor

    property font lrcFont: Setting.lrcFont
    property font deskLrcFont: Setting.deskLrcFont
}
