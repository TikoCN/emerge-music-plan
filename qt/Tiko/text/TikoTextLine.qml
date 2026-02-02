import QtQuick
import QtQuick.Controls
import Tiko

Text {
    property Rectangle background: background
    property int exSize: 0

    id: root
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    color: TikoSeit.transparentColor
    font: TikoSeit.textNormlFont
    height: font.pixelSize * 1.3 + 2 * TikoSeit.normalMargins

    Rectangle {
        id: background
        anchors.fill: parent
        color: TikoSeit.transparentColor
        opacity: 0
    }
}
