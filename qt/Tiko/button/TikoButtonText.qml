import QtQuick
import Tiko

TikoButtonBase {
    id: textBaseButton
    implicitHeight: textLineItem.height

    property TikoTextLine textLine: textLineItem

    TikoTextLine {
        width: parent.width
        id: textLineItem
        color: showColor
    }
}
