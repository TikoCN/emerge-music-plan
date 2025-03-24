import QtQuick
import QtQuick.Controls.Basic
import Tiko

MenuItem {
    id: root
    font.family: TikoSeit.fontFamily
    font.pixelSize: TikoSeit.fontPixelSize
    implicitHeight: 35

    //背景
    background: Rectangle{
        anchors.fill: parent
        color: TikoSeit.transparentColor
        opacity: root.hovered ? 0.05 : 0
    }

    //内容
    contentItem: Item{
        Image {
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
