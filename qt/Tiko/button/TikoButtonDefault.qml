import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    width: iconItem.width + textLineItem.width + TikoSeit.subitemSpace * 3
    height: 50
    property TikoImage icon: iconItem
    property TikoTextLine textLine: textLineItem
    property bool hideEmptyIcon: true

    onHideEmptyIconChanged: updateIconVisibility()

    function updateIconVisibility() {
        iconItem.visible = !hideEmptyIcon || iconItem.source.toString().length > 0
    }

    TikoImage {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
        onSourceChanged: normalButton.updateIconVisibility()
    }

    TikoTextLine {
        id: textLineItem
        text: "TikoButtonDefault"
        anchors.left: iconItem.right
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
    }
}
