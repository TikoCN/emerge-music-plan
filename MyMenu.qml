import QtQuick
import QtQuick.Controls.Basic
import MyAPI

Menu {
    implicitWidth: 300
    font.family: Setting.mainFont.family
    font.pixelSize: Setting.mainFont.pixelSize
    topPadding: 5
    bottomPadding: 5

    //背景
    background: Rectangle{
        color: Setting.backdropColor
        radius: 10

        Rectangle{
            anchors.fill: parent
            border.color: Setting.transparentColor
            radius: 10
            border.width: 0.5
            opacity: 0.3
        }
    }

    delegate: MyMenuItem {}
}
