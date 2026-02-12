import QtQuick

QtObject {
    property color hoverColor: Qt.rgba(1, 0, 0, 0.7)
    property color pressedColor: Qt.rgba(1, 0, 0, 0.8)
    property color disabledColor: Qt.rgba(0, 0, 0, 0.1)
    property color normalColor: Qt.rgba(0, 0, 0, 1)

    property bool isNull: false
    function setBenchmarkColor(benchmarkColor, isLightTheme) {
        if (isNull) return

        let hslHue = benchmarkColor.hslHue
        let hslSaturation = benchmarkColor.hslSaturation
        let hslLightness = benchmarkColor.hslLightness
        let hoverLightness, pressedLightness
        let a = benchmarkColor.a

        if (isLightTheme) {
            hoverLightness = 0.5   // 中等亮度
            pressedLightness = 0.35  // 比hover暗
        } else {
            hoverLightness = 0.5   // 中等亮度
            pressedLightness = 0.25  // 比hover暗
        }

        // Hover - 恢复部分基准色特征
        hoverColor = Qt.hsla(
            (hslHue + 0.02) % 1,
            Math.min(1, hslSaturation * 0.6),  // 恢复部分饱和度
            hoverLightness,
            a
        )

        // Pressed
        pressedColor = Qt.hsla(
            (hslHue - 0.01) % 1,
            Math.min(1, hslSaturation * 0.8),  // 恢复更多饱和度
            pressedLightness,
            a
        )

        // Disabled - 接近中性色
        disabledColor = Qt.hsla(
            hslHue,
            hslSaturation * 0.15,
            isLightTheme ? 0.6 : 0.4,  // 亮主题用浅灰，暗主题用深灰
            a * 0.6
        )
    }

    function setNullColor(){
        isNull = true

        hoverColor = Qt.rgba(0, 0, 0, 0)
        pressedColor = Qt.rgba(0, 0, 0, 0)
        disabledColor = Qt.rgba(0, 0, 0, 0)
    }
}
