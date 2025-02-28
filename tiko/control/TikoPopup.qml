import QtQuick
import QtQuick.Controls
import "../"
Popup {
    id: popup
    parent: Overlay.overlay
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2

    property double radius: 0
    property color borderColor: TikoSeit.transparentColor
    property color backColor: TikoSeit.backdropColor

    background: Rectangle{
        color: popup.backColor
        radius: popup.radius

        Rectangle{
            anchors.fill: parent
            radius: popup.radius
            color: "#00000000"
            border.color: popup.borderColor
            border.width: 0.5
            opacity: 0.3
        }
    }
}
