import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    implicitHeight: textLineItem.height + TikoSeit.normalMargins * 2
    implicitWidth: childrenRect.width
    icon: iconItem
    textLine: textLineItem

    TikoDynamicIcon {
        id: iconItem
        dynamicState: normalButton.dynamicState
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: textLine.height + TikoSeit.subitemSpace
        width: height
    }

    TikoDynamicTextLine {
        id: textLineItem
        dynamicState: normalButton.dynamicState
        anchors.left: iconItem.right
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
    }
}
