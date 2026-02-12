import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    property int outHeight : 0
    implicitHeight: Math.max(iconItem.implicitHeight, textLineItem.implicitHeight) + outHeight
    implicitWidth: childrenRect.width
    icon: iconItem
    textLine: textLineItem

    TikoDynamicIcon {
        id: iconItem
        dynamicState: normalButton.dynamicState
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        height: textLine.height + 16
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
