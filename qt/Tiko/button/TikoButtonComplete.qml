import QtQuick
import Tiko

TikoButtonBase {
    id: completeButton
    implicitHeight: childrenRect.height + TikoSeit.normalMargins * 2
    implicitWidth: childrenRect.width + TikoSeit.normalMargins * 2
    icon: iconItem
    textLine: textLineItem
    background: backgroundItem

    TikoDynamicBackGround {
        id: backgroundItem
        dynamicState: completeButton.dynamicState
        anchors.fill: parent
    }

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
}
