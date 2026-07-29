import QtQuick
import QtQuick.Controls.Basic
import Tiko
import QtQuick.Effects

MenuItem {
    id: root
    font: TikoSeit.theme.fontDefault
    implicitHeight: 35

    property color disabledColor: TikoSeit.theme.colorFgHint
    property bool disable: true

    //背景
    background: Rectangle {
        id: tspItem
        radius: 5
        anchors.fill: parent
        color: TikoSeit.theme.colorMaxTop
        opacity: hovered ? 0.08 : 0
    }

    //内容
    contentItem: Item {

        TikoImage {
            id: icon
            height: parent.height
            width: parent.height
            anchors.verticalCenter: parent.verticalCenter
            source: root.icon.source
        }
        TikoTextLine{
            text: root.text
            anchors.left: icon.right
            anchors.right: parent.right
            anchors.margins: TikoSeit.subitemSpace
            anchors.verticalCenter: parent.verticalCenter
            color: disable ? TikoSeit.theme.colorTextDefault : disabledColor
        }
    }
}
