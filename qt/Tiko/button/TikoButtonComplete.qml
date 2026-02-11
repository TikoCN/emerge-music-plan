import QtQuick
import Tiko

TikoButtonBase {
    id: completeButton
    implicitHeight: childrenRect.height
    implicitWidth: childrenRect.width
    icon: iconItem
    textLine: textLineItem
    background: backgroundItem

    TikoImage {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.subitemSpace
    }

    TikoTextLine {
        id: textLineItem
        anchors.left: iconItem.right
        anchors.verticalCenter: iconItem.verticalCenter
        anchors.margins: TikoSeit.subitemSpace
        outWidth: TikoSeit.subitemSpace
    }

    TikoDynamicBackGround {
        id: backgroundItem
        dynamicState: completeButton.dynamicState
        anchors.fill: parent
        z: -1
    }
}
