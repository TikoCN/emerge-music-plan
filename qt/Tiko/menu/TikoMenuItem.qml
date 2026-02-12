import QtQuick
import QtQuick.Controls.Basic
import Tiko
import QtQuick.Effects

MenuItem {
    id: root
    font: TikoSeit.theme.textTheme.normalFont
    implicitHeight: 35

    property color unifiedColor: TikoSeit.theme.baseTheme.backgroundNormal
    property bool useUnifiedColor: true

    //背景
    background: Rectangle{
        anchors.fill: parent
        color: root.hovered ? TikoSeit.theme.baseTheme.borderNormal : TikoSeit.theme.baseTheme.backgroundNormal
    }

    //内容
    contentItem: Rectangle {
        color: "#00000000"
        opacity: root.enabled ? 1 : 0.4

        Image {
            id: icon
            height: parent.height
            width: parent.height
            anchors.verticalCenter: parent.verticalCenter
            source: root.icon.source
            visible: false
        }
        MultiEffect {
            id: iconShow
            anchors.fill: icon
            source: icon
            colorization: useUnifiedColor ? 1 : 0
            colorizationColor: unifiedColor
        }
        TikoTextLine{
            text: root.text
            width: parent.width - icon.width
            x: 35
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
