import QtQuick
import QtQuick.Controls

Popup {
    id: tikoPopup
    y: parent.height + 10
    property double radius: 0

    background: Rectangle{
        color: TikoSeit.backdropColor
        radius: tikoPopup.radius

        Rectangle{
            anchors.fill: parent
            radius: tikoPopup.radius
            border.color: TikoSeit.transparentColor
            border.width: 0.5
            opacity: 0.3
        }
    }
}
