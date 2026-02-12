import QtQuick
import QtQuick.Controls.Basic
import Tiko

TikoButtonComplete {
    id: completeButton
    implicitHeight: textLineItem.height + TikoSeit.normalMargins * 2
    implicitWidth: textLineItem.width + TikoSeit.normalMargins * 2
    textLine: textLineItem
    background: backgroundItem

    TikoDynamicTextLine {
        id: textLineItem
        anchors.centerIn: completeButton
    }

    TikoDynamicBackGround {
        id: backgroundItem
        dynamicState: completeButton.dynamicState
        anchors.fill: parent
        z: -1
    }

    Component.onCompleted: {
        background.backgroundDynamicColor.setNullColor()
    }
}
