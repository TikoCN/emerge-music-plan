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
    text: "TikoTextLine"
    property int outWidth: 0
    property int level: 1
    property int addFontSize: 0

    color: TikoSeit.theme.colorTextDefault
    font.family: TikoSeit.theme.fontDefault.family
    font.pointSize: TikoSeit.theme.fontDefault.pointSize + addFontSize

    onLevelChanged: {
        switch (level) {
        case 0:
            addFontSize = -1
            opacity = 0.6
            textLine.font.bold = false
            return
        case 1:
            addFontSize = 0
            opacity = 1
            textLine.font.bold = false
            return
        case 2:
            addFontSize = 2
            opacity = 1
            textLine.font.bold = true
            return
        }
    }
}
