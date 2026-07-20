import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    width: 250
    height: 50
    property TikoImage icon: iconItem
    property TikoTextLine textLine: textLineItem


    TikoImage {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
    }

    TikoTextLine {
        id: textLineItem
        text: "TikoButtonDefault"
        anchors.left: iconItem.right
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
    }
}
