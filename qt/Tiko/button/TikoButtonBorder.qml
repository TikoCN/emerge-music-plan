import QtQuick
import QtQuick.Controls.Basic
import Tiko

TikoButtonComplete {
    id: completeButton
    implicitHeight: textLineItem.height + TikoSeit.normalMargins * 2
    implicitWidth: textLineItem.width + TikoSeit.normalMargins * 2
    textLine: textLineItem
    background: backgroundItem

    TikoDynamicBackGround {
        id: backgroundItem
        dynamicState: completeButton.dynamicState
        anchors.fill: parent
    }

    TikoDynamicTextLine {
        id: textLineItem
        dynamicState: completeButton.dynamicState
        anchors.centerIn: completeButton
    }

    Component.onCompleted: {
        background.backgroundDynamicColor.setNullColor()
    }
}
