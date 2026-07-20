import QtQuick
import QtQuick.Controls
import Tiko

Popup {
    id: popup
    onClosed: hide()

    property double radius: 0
    property double backOpacity: 1
    signal hide()

    background: Rectangle{
        color: TikoSeit.theme.colorMaxBottom
        radius: popup.radius
        opacity: popup.backOpacity

        Rectangle{
            anchors.fill: parent
            radius: popup.radius
            color: "#00000000"
            border.color: TikoSeit.theme.colorBgDefault
            border.width: 0.5
        }
    }
}
