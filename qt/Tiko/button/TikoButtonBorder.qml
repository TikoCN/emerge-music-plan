import QtQuick
import QtQuick.Controls.Basic
import Tiko

TikoButtonBase {
    id: button
    width: textRect.boundingRect.width * 4
    height: 32

    property color backColor: TikoSeit.transparentColor
    property color textColor: TikoSeit.backdropColor
    property string text: qsTr("TikoButtonBorder")
    property int radius: 20
    onHoveredChanged: {
        if (hovered)
            inAnimation.start()
        else
            outAnimation.start()
    }

    Rectangle {
        id: back
        radius: radius
        color: button.backColor
        opacity: 0.3
        anchors.fill: parent

        PropertyAnimation on radius {
            id: inAnimation
            from: button.radius
            to: 0
            duration: 300
        }

        PropertyAnimation on radius {
            id: outAnimation
            from: button.radius
            to: 0
            duration: 300
        }
    }

    TikoTextLine {
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
