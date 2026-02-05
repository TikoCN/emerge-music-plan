import QtQuick
import Tiko

TikoButtonBase {

    property bool isUseShowColor: true
    property TikoTextLine textLine: textLineItem
    property TikoImage icon: iconImg

    TikoImage {
        id: iconImg
        autoColor: showColor
        isUseAutoColor: isUseShowColor

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: TikoSeit.normalMargins


        anchors.right: textLineItem.left
        anchors.rightMargin: TikoSeit.subitemSpace
    }

    TikoTextLine {
        id: textLineItem
        autoColor: showColor
        isUseAutoColor: isUseShowColor

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: TikoSeit.normalMargins


        anchors.left: iconImg.right
        anchors.leftMargin: TikoSeit.subitemSpace
    }
}
