import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    implicitHeight: Math.max(iconItem.implicitHeight, textLineItem.implicitHeight)
    implicitWidth: childrenRect.width
    icon: iconItem
    textLine: textLineItem

    TikoDynamicIcon {
        id: iconItem
        dynamicState: normalButton.dynamicState
        anchors.left: parent.left
    }


    TikoDynamicTextLine {
        id: textLineItem
        dynamicState: normalButton.dynamicState
        anchors.left: iconItem.right
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: iconItem.verticalCenter
    }
}
