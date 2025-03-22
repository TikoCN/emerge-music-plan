import QtQuick
import QtQuick.Controls.Basic
import Tiko

Button {
    id: button
    implicitWidth: textRect.boundingRect.width * 4
    implicitHeight: 32
    property color backColor: TikoSeit.transparentColor
    property color textColor: TikoSeit.backdropColor
    property int radius: 3

    background: Rectangle{
        id: back
        radius: button.radius
        color: button.backColor
        opacity: 0.3
    }

    contentItem: TikoTextLine{
        id: textShow
        anchors.fill: parent
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: button.textColor
    }

    TextMetrics{
        id: textRect
        font: textShow.font
        text: textShow.text
    }
}
