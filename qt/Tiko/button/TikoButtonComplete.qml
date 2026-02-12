import QtQuick
import Tiko

TikoButtonBase {
    id: completeButton
    implicitHeight: childrenRect.height
    implicitWidth: childrenRect.width
    icon: iconItem
    textLine: textLineItem
    background: backgroundItem

    TikoDynamicIcon {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.subitemSpace
    }

    TikoDynamicTextLine {
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
