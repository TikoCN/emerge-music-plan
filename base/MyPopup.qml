import QtQuick
import QtQuick.Controls

Popup {
    y: parent.height + 10

    background: Rectangle{
        color: BaseSeit.backdropColor
        radius: 10

        Rectangle{
            anchors.fill: parent
            radius: 10
            border.color: BaseSeit.transparentColor
            border.width: 0.5
            opacity: 0.3
        }
    }
}
