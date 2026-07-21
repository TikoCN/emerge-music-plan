import QtQuick
import QtQuick.Controls
import Tiko
import QtQuick.Effects

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
            color: TikoSeit.theme.colorBgView
            layer.enabled: true
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowBlur: 0.5
                shadowColor: TikoSeit.theme.colorFgDefault
                shadowHorizontalOffset: 0
                shadowVerticalOffset: 0
            }
            radius: 10
        }
    }
}
