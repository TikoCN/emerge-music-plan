import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: normalButton
    height: 50
    width: implicitWidth
    implicitWidth: textLineItem.naturalWidth + textLineItem.anchors.leftMargin + TikoSeit.subitemSpace
                   + (hasVisibleIcon ? iconItem.anchors.leftMargin + iconItem.width : 0)
    property TikoImage icon: iconItem
    property TikoTextLine textLine: textLineItem
    property bool hideEmptyIcon: true
    readonly property bool hasVisibleIcon: !hideEmptyIcon || iconItem.source.toString().length > 0

    TikoImage {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
        visible: normalButton.hasVisibleIcon
    }

    TikoTextLine {
        id: textLineItem
        text: "TikoButtonDefault"
        anchors.left: normalButton.hasVisibleIcon ? iconItem.right : normalButton.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.verticalCenter: parent.verticalCenter
        width: normalButton.width > 0
               ? Math.max(0, normalButton.width - x - TikoSeit.subitemSpace)
               : naturalWidth
    }
}
