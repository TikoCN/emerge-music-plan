import QtQuick.Controls.Basic
import QtQuick

Button {
    id: uiButton
    implicitWidth: 32
    implicitHeight: 32
    icon.width: 20
    icon.height: 20
    padding: 0
    onHoveredChanged: {
        if(uiButton.hovered){
            rotAnimation.start()
        }
    }

    property int radius: 3
    property double hover: 0.3
    property double normal: 0
    property bool cache: false

    background: Rectangle{
        color: TikoSeit.transparentColor//背景颜色
        radius: uiButton.radius
        opacity: {
            return uiButton.hovered ? uiButton.hover :  uiButton.normal
        }
    }

    contentItem: Item{
        id: showItem
        Image{
            id: iconImg
            width: uiButton.icon.width
            height: uiButton.icon.height
            source: uiButton.icon.source
            cache: uiButton.cache
            y: (showItem.height - iconImg.height) / 2
            anchors.horizontalCenter: showItem.horizontalCenter
        }
    }

    //旋转动画
    RotationAnimation{
        id: rotAnimation
        target: iconImg
        from: 0
        to: 360
        duration: 500
    }
}
