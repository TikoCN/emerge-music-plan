import QtQuick
import QtQuick.Controls.Basic

Menu {
    implicitWidth: 300
    font.family: BaseSeit.fontFamily
    font.pixelSize: BaseSeit.fontPixelSize
    topPadding: 5
    bottomPadding: 5

    //背景
    background: Rectangle{
        color: BaseSeit.backdropColor
        radius: 10

        Rectangle{
            anchors.fill: parent
            border.color: BaseSeit.transparentColor
            radius: 10
            border.width: 0.5
            opacity: 0.3
        }
    }

    delegate: MyMenuItem {}
}
