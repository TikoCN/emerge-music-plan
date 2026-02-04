import QtQuick
import Tiko

TikoButtonBase {
    id: textBaseButton
    height: textLineItem.height
    property string text: "button"
    property TikoTextLine textLine: textLineItem
    property TikoImageColorAuto autoImage: autoImageItem

    TikoTextLine {
        id: textLineItem
        color: showColor
    }

    TikoImageAuto {
        id: autoImageItem
        autoColor: showColor
    }
}
