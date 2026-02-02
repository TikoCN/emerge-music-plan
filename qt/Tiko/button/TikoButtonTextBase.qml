import QtQuick
import Tiko

TikoButtonBase {
    id: textBaseButton
    height: textLineItem.height
    property string text: "button"
    property TikoTextLine textLine: textLineItem

    TikoTextLine {
        id: textLineItem
    }
}
