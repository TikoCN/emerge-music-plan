import QtQuick
import QtQuick.Controls.Basic

Button {
    id: button
    implicitWidth: textRect.boundingRect.width * 4
    implicitHeight: 32
    property color color: TikoSeit.themeColor
    property int radius: 3

    background: Rectangle{
        id: back
        radius: button.radius
        color: "#00000000"
        border.color: button.color
        border.width: 1.5
        opacity: 0.3
    }

    Rectangle{
        anchors.fill: parent
        radius: button.radius
        color: button.color
        opacity: 0.05
    }

    PropertyAnimation{

    }

    contentItem: TikoAutoText{
        id: textShow
        anchors.fill: parent
        text: button.text
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: button.color
        opacity: 0.5
        font.bold: true
    }

    TextMetrics{
        id: textRect
        font: textShow.font
        text: textShow.text
    }
}
