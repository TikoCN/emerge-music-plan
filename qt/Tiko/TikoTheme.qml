import QtQuick
import Tiko
import MediaerAPI

QtObject {
    // 主题管理
    property TikoThemeBackground baseTheme: TikoThemeBackground{}

    // 动态颜色
    property TikoDynamicColor foregroundDynamicColor: TikoDynamicColor{}
    property TikoDynamicColor backgroundDynamicColor: TikoDynamicColor{}
    property TikoDynamicColor borderDynamicColor: TikoDynamicColor{}

    property TikoTextLineType textTheme: TikoTextLineType{}

    function setDynamicColor(bechmarkColor, isLightTheme){
        let colors = generateThemeColors(bechmarkColor, isLightTheme)
        backgroundDynamicColor.setBenchmarkColor(colors.background, isLightTheme)
        borderDynamicColor.setBenchmarkColor(colors.border, isLightTheme)
        foregroundDynamicColor.setBenchmarkColor(colors.foreground, isLightTheme)
    }

    // 根据基准色和主题生成三种核心颜色
    function generateThemeColors(baseColor, isLightTheme) {
        if (!baseColor) {
            console.error("基准颜色不能为空")
            return null
        }

        // 从基准色提取HSL值
        let baseHue = baseColor.hslHue
        let baseSaturation = baseColor.hslSaturation
        let baseLightness = baseColor.hslLightness
        let alpha = baseColor.a

        let colors = {}

        if (isLightTheme) {
            // ========== 亮色主题（白底黑字）==========

            // 背景色：接近白色，保留轻微色调
            colors.background = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        baseSaturation * 0.1,       // 大幅降低饱和度，接近白色
                        Math.max(0.95, baseLightness * 1.1),  // 提高亮度，接近白色
                        alpha
                        )

            // 边框色：中性灰色，轻微色调
            colors.border = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        baseSaturation * 0.15,      // 降低饱和度
                        Math.min(0.8, Math.max(0.4, baseLightness * 0.7)),  // 中等亮度
                        alpha
                        )

            // 前景色：接近黑色，保留适当色调
            colors.foreground = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        Math.min(1, baseSaturation * 0.6),  // 适度饱和度
                        Math.min(0.2, baseLightness * 0.5),  // 降低亮度，接近黑色
                        alpha
                        )

        } else {
            // ========== 暗色主题（黑底白字）==========

            // 背景色：接近黑色，保留轻微色调
            colors.background = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        baseSaturation * 0.1,       // 大幅降低饱和度，接近黑色
                        Math.min(0.1, baseLightness * 0.5),  // 降低亮度，接近黑色
                        alpha
                        )

            // 边框色：中性灰色，轻微色调
            colors.border = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        baseSaturation * 0.15,      // 降低饱和度
                        Math.max(0.3, Math.min(0.7, baseLightness * 1.3)),  // 中等亮度
                        alpha
                        )

            // 前景色：接近白色，保留适当色调
            colors.foreground = Qt.hsla(
                        baseHue,                    // 保持基准色调
                        Math.min(1, baseSaturation * 0.6),  // 适度饱和度
                        Math.max(0.9, baseLightness * 1.1),  // 提高亮度，接近白色
                        alpha
                        )
        }

        return colors
    }
}
