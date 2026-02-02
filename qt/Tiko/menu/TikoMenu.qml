import QtQuick
import QtQuick.Controls.Basic
import Tiko

Menu {
    implicitWidth: 300
    font: TikoSeit.textNormlFont
    topPadding: 5
    bottomPadding: 5

    //背景
    background: Rectangle{
        color: TikoSeit.backdropColor
        radius: 10

        Rectangle{
            anchors.fill: parent
            border.color: TikoSeit.transparentColor
            radius: 10
            border.width: 0.5
            opacity: 0.3
        }
    }

    delegate: TikoMenuItem {}
}
