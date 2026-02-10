import QtQuick
import Tiko

TikoButtonBase {
    width: textLineItem.width + iconImg.width + TikoSeit.subitemSpace + TikoSeit.normalMargins * 2
    height: Math.max(textLineItem.height+ TikoSeit.normalMargins * 2, 50)
    property bool isUseShowColor: true
    property TikoTextLine textLine: textLineItem
    property TikoImage icon: iconImg

    TikoImage {
        id: iconImg
        unifiedColor: showColor
        enableUnifiedColor: isUseShowColor

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: TikoSeit.normalMargins
    }

    TikoTextLine {
        id: textLineItem
        unifiedColor: showColor
        enableUnifiedColor: isUseShowColor

        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: TikoSeit.normalMargins


        anchors.left: iconImg.right
        anchors.leftMargin: TikoSeit.subitemSpace
    }
}
