import QtQuick
import QtQuick.Controls
import Tiko

Text {
    id: root
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    clip: true
    height: font.pixelSize
    width: text.length * font.pixelSize
    enum TextType {
        NORMAL = 0,
        TITLE = 1,
        SUBTITLE = 2,
        INFOR = 3,
        ASSISTANCE = 4,
        DISABLE = 5
    }
    property var textType: TikoTextLine.TextType.NORMAL
    property color normalColor: { switch (textType) {
        case TikoTextLine.TextType.TITLE:
            return TikoSeit.textTitleColor
        case TikoTextLine.TextType.SUBTITLE:
            return TikoSeit.textSubtitleColor
        case TikoTextLine.TextType.DISABLE:
            return TikoSeit.textDisabledColor
        case TikoTextLine.TextType.INFOR:
            return TikoSeit.textInformationColor
        case TikoTextLine.TextType.ASSISTANCE:
        case TikoTextLine.TextType.NORMAL:
        default:
            return TikoSeit.textNormalColor
        }
    }

    color: normalColor

    font: {
        switch (textType) {
        case TikoTextLine.TextType.TITLE:
            return TikoSeit.titleFont
        case TikoTextLine.TextType.SUBTITLE:
            return TikoSeit.subtitleFont
        case TikoTextLine.TextType.INFOR:
            return TikoSeit.informationFont
        case TikoTextLine.TextType.ASSISTANCE:
            return TikoSeit.assistanceFont
        case TikoTextLine.TextType.DISABLE:
        case TikoTextLine.TextType.NORMAL:
        default:
            return TikoSeit.textNormalFont
        }
    }
}
