import QtQuick
import QtQuick.Controls
import MyAPI

Popup {
    y: parent.height + 10

    background: Rectangle{
        color: Setting.backdropColor
        radius: 10

        Rectangle{
            anchors.fill: parent
            radius: 10
            border.color: Setting.transparentColor
            border.width: 0.5
            opacity: 0.3
        }
    }
}
