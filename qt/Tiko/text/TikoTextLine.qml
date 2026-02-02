import QtQuick
import QtQuick.Controls
import Tiko

Text {
    id: root
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter

    enum TextType {
        NORMAL = 0,
        TITLE = 1,
        SUBTITLE = 2,
        INFOR = 3,
        ASSISTANCE = 4,
        DISABLE = 5
    }
    property int textType: Number(TextType.NORMAL)
    property Rectangle background: background
    property int exSize: 0

    color: {
        switch (textType) {
        case TextType.TITLE:
            return TikoSeit.textTitleColor
        case TextType.SUBTITLE:
            return TikoSeit.textSubtitleColor
        case TextType.DISABLE:
            return TikoSeit.textDisabledColor
        case TextType.INFOR:
            return TikoSeit.textInformationColor
        case TextType.ASSISTANCE:
        case TextType.NORMAL:
        default:
            return TikoSeit.textNormalColor
        }
    }

    font: {
        switch (textType) {
        case TextType.TITLE:
            return TikoSeit.titleFont
        case TextType.SUBTITLE:
            return TikoSeit.subtitleFont
        case TextType.INFOR:
            return TikoSeit.informationFont
        case TextType.ASSISTANCE:
            return TikoSeit.assistanceFont
        case TextType.DISABLE:
        case TextType.NORMAL:
        default:
            return TikoSeit.textNormalFont
        }
    }
    height: font.pixelSize * 1.3 + 2 * TikoSeit.normalMargins

    Rectangle {
        id: background
        anchors.fill: parent
        color: TikoSeit.transparentColor
        opacity: 0
    }
}
