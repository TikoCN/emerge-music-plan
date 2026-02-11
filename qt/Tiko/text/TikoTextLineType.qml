import QtQuick
import Tiko

QtObject {
    property color normalColor: Qt.rgba(0, 0, 0, 1)
    property color titleColor: Qt.rgba(0, 0, 0, 1)
    property color subtitleColor: Qt.rgba(0, 0, 0, 1)
    property color informationColor: Qt.rgba(0, 0, 0, 1)
    property color assistanceColor: Qt.rgba(0, 0, 0, 1)


    property font normalFont:  Qt.font({family: "Microsoft YaHei", pointSize: 12})
    property font titleFont: Qt.font({family: "Microsoft YaHei", pointSize: 14})
    property font subtitleFont: Qt.font({family: "Microsoft YaHei", pointSize: 12})
    property font informationFont: Qt.font({family: "Microsoft YaHei", pointSize: 10})
    property font assistanceFont: Qt.font({family: "Microsoft YaHei", pointSize: 10})

    function setColor(isLight) {
        if (isLight) {
            // 亮主题：
            normalColor = Qt.rgba(0.1, 0.1, 0.1, 1) // 黑色
            titleColor = Qt.rgba(0, 0, 0, 1) // 黑色
            subtitleColor = Qt.rgba(0.3, 0.3, 0.3, 1) // 深灰色
            informationColor = Qt.rgba(0.5, 0.5, 0.5, 1) // 灰色
            assistanceColor = Qt.rgba(0.7, 0.7, 0.7, 1) // 浅灰色
        } else {
            // 暗主题：
            normalColor = Qt.rgba(0.9, 0.9, 0.9, 1) // 白色
            titleColor = Qt.rgba(1, 1, 1, 1) // 白色
            subtitleColor = Qt.rgba(0.8, 0.8, 0.8, 1) // 浅灰色
            informationColor = Qt.rgba(0.6, 0.6, 0.6, 1) // 中灰色
            assistanceColor = Qt.rgba(0.4, 0.4, 0.4, 1) // 深灰色
        }
    }

    function setFont(font){
        normalFont = font
        titleFont = font
        titleFont.pointSize += 2
        subtitleFont = font
        informationFont = font
        informationFont.pointSize -= 2
        assistanceFont = informationFont
    }
}
