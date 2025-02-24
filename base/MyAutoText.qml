import QtQuick
import QtQuick.Controls
import MyAPI

Text {
    property Rectangle background: background
    property int exSize: 0
    property font allFont: Setting.mainFont

    id: root
    elide: Text.ElideRight
    verticalAlignment: Text.AlignVCenter
    color: Setting.transparentColor//背景颜色
    font.family: allFont.family
    font.pixelSize: allFont.pixelSize + exSize

    Rectangle {
        id: background
        anchors.fill: parent
        color: Setting.transparentColor
        opacity: 0
    }
}
