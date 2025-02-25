import QtQuick
import QtQuick.Controls

Text {
    property Rectangle background: background
    property int exSize: 0

    id: root
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    color: BaseSeit.transparentColor//背景颜色
    font.family: BaseSeit.fontFamily
    font.pixelSize: BaseSeit.fontPixelSize + exSize

    Rectangle {
        id: background
        anchors.fill: parent
        color: BaseSeit.transparentColor
        opacity: 0
    }
}
