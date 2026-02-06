import QtQuick
import QtQuick.Controls.Basic
import Tiko

TikoButtonComplete {
    id: button
    implicitWidth: textLine.width
    isUseShowColor: true

    Rectangle {
        id: back
        radius: 8
        border.color: showColor
        opacity: 0.3
        anchors.fill: parent
        z: -1
    }
}
