import QtQuick
import Tiko

QtObject {
    property bool isDark: true

    property color colorMaxBottom: Qt.rgba(1, 1, 1, 1.0)
    property color colorMaxTop: Qt.rgba(0, 0, 0, 1.0)
    property color colorTextDefault: Qt.rgba(0.03,0.03,0.03,1)
    property color colorFgDefault: Qt.rgba(0.18,0.18,0.18,1)
    property color colorFgHint: Qt.rgba(0.40,0.40,0.40,1)
    property color colorBgDefault: Qt.rgba(227/255.0, 227/255.0, 227/255.0, 1)
    property color colorBgView: Qt.rgba(250/255.0, 250/255.0, 251/255.0, 1)
    property color colorBgHint: Qt.rgba(200/255.0, 200/255.0, 200/255.0, 1)
    property color colorHighlight: Qt.rgba(0/255.0, 191/255.0, 165/255.0, 1)

    property font fontDefault:  Qt.font({family: "Microsoft YaHei", pointSize: 12})

    function switchTheme(flag) {

    }
}
