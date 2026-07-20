import QtQuick
import QtQuick.Controls
import Tiko

Rectangle {
    id: root
    property font selectedFont: Qt.font
    property string text
    border.width: 3
    radius: 10
    implicitHeight: 60
    implicitWidth: 100
}
