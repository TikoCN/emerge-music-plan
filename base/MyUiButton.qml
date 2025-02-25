import QtQuick.Controls.Basic
import QtQuick

Button {
    id: button
    implicitWidth: 32
    implicitHeight: 32
    icon.width: 20
    icon.height: 20

    property int radius:  3
    property double hover:  0.3
    property double normal:  0
    property bool cache: true


    ToolTip.delay: 1000
    ToolTip.timeout: 5000
    ToolTip.visible: button.hovered
    ToolTip.text: text

    background: Rectangle{
        color: BaseSeit.transparentColor//背景颜色
        radius: button.radius
        opacity: {
            return button.hovered ? button.hover :  button.normal
        }
    }

    contentItem: Item{
        Image{
            id: iconImg
            width: icon.width
            height: icon.height
            anchors.centerIn: parent
            source: button.icon.source
            cache: button.cache
        }
    }
}
