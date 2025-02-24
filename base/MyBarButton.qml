import QtQuick.Controls.Basic
import QtQuick
import MyAPI

Button {
    id: button
    implicitWidth: 40
    implicitHeight: 40
    icon.width: 20
    icon.height: 20

    property int radius: 10
    property double hover: 0.3
    property double normal: 0

    background: Rectangle{
        color: Setting.transparentColor//背景颜色
        radius: button.radius
        opacity: {
            return button.hovered ? button.hover :  button.normal
        }
    }

    contentItem: Item {
        Image{
            id: iconImg
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: button.icon.source
            sourceSize.width: width
            sourceSize.height: height
            width: button.icon.width
            height: button.icon.height
        }
        MyAutoText{
            id: textShow
            anchors.left: iconImg.right
            anchors.leftMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            text: button.text
            width: button.width - iconImg.width - 20
        }
    }
}
