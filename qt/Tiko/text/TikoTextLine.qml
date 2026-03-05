import QtQuick
import QtQuick.Controls
import Tiko

Text {
    id: textLine
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    clip: true
    height: font.pixelSize * 1.5
    width: text.length * font.pixelSize + outWidth
    property int outWidth: 0

    color: TikoSeit.theme.textTheme.normalColor
    font: TikoSeit.theme.textTheme.normalFont
}
