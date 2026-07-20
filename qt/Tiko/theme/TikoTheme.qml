import QtQuick
import Tiko

QtObject {
    property bool isDark: true

    property color colorMaxBottom: Qt.rgba(30/255, 30/255, 36/255, 1.0)
    property color colorMaxTop: Qt.rgba(1, 1, 1, 1.0)
    property color colorTextDefault: Qt.rgba(0.97,0.97,0.97,1)
    property color colorFgDefault: Qt.rgba(0.97,0.97,0.97,1)
    property color colorFgHint: Qt.rgba(0.90,0.90,0.90,0.6)
    property color colorBgDefault: Qt.rgba(37/255, 39/255, 42/255, 1)
    property color colorHighlight: Qt.rgba(0/255, 191/255, 165/255, 1)

    property font fontDefault:  Qt.font({family: "Microsoft YaHei", pointSize: 12})

    function switchTheme(flag) {

    }
}
