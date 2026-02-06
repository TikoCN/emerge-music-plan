import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonComplete {
    id: button

    onIsHoverChanged: {
        if(button.isHover){
            hoveringAnimation.start()
        }
        else{
            hoveredAnimation.start()
        }
    }

    property int radius: 10
    property double borderWidth: 1
    property color backColor: showColor

    //背景颜色
    Rectangle{
        id: backRect
        color: backColor
        radius: button.radius
        anchors.fill: parent
        opacity: 0
    }

    //鼠标进入
    PropertyAnimation{
        id: hoveringAnimation
        target: backRect
        properties: "opacity"
        from: button.normal
        to: button.hover
        duration: 300
    }

    //鼠标退出
    PropertyAnimation{
        id: hoveredAnimation
        target: backRect
        properties: "opacity"
        from: button.hover
        to: button.normal
        duration: 300
    }
}
