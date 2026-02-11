import QtQuick
import QtQuick.Controls
import Tiko

Text {
    id: textLine
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    clip: true
    height: font.pixelSize
    width: minWidth + outWidth
    property int minWidth: text.length * font.pixelSize
    property int outWidth: 0

    color: TikoSeit.theme.textTheme.normalColor
    font: TikoSeit.theme.textTheme.normalFont
}
