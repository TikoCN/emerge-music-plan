import QtQuick
import QtQuick.Controls.Basic
import Tiko
import QtQuick.Effects

MenuItem {
    id: root
    font: TikoSeit.theme.fontDefault
    implicitHeight: 35

    property color unifiedColor: TikoSeit.theme.colorBgDefault
    property bool useUnifiedColor: true

    //背景
    background: Rectangle{
        anchors.fill: parent
        color: root.hovered ? TikoSeit.theme.colorBrandSecondary : TikoSeit.theme.colorBgDefault
    }

    //内容
    contentItem: Rectangle {
        color: "#00000000"
        opacity: root.enabled ? 1 : 0.4

        TikoImage {
            id: icon
            height: parent.height
            width: parent.height
            anchors.verticalCenter: parent.verticalCenter
            source: root.icon.source
        }
        TikoTextLine{
            text: root.text
            width: parent.width - icon.width
            x: 35
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
