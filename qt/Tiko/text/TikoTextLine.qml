import QtQuick
import QtQuick.Controls
import Tiko

Text {
    id: textLine
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    clip: true
    height: textLine.font.pixelSize * 1.5
    // 独立测量完整文字，避免省略显示和父项宽度约束影响自然宽度。
    width: naturalWidth
    text: "TikoTextLine"
    property int outWidth: 0
    readonly property real naturalWidth: Math.ceil(Math.max(textMetrics.advanceWidth, textMetrics.boundingRect.width)) + outWidth
    property int level: 1
    property int addFontSize: 0

    color: TikoSeit.theme.colorTextDefault
    font.family: TikoSeit.theme.fontDefault.family
    font.pointSize: TikoSeit.theme.fontDefault.pointSize + addFontSize

    TextMetrics {
        id: textMetrics
        font: textLine.font
        text: textLine.text
    }

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
