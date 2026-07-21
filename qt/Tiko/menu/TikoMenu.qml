import QtQuick
import QtQuick.Controls.Basic
import Tiko
import QtQuick.Effects

Menu {
    implicitWidth: 300
    font: TikoSeit.theme.fontDefault

    //背景
    background: Rectangle{
        color: TikoSeit.theme.colorMaxBottom
        radius: 10

        Rectangle {
            anchors.fill: parent
            color: TikoSeit.theme.colorBgView
            layer.enabled: true
            layer.effect: MultiEffect {
                shadowEnabled: true
                shadowBlur: 0.5
                shadowColor: TikoSeit.theme.colorBgDefault
                shadowHorizontalOffset: 0
                shadowVerticalOffset: 0
            }
            radius: 10
        }
    }

    delegate: TikoMenuItem {}
}
