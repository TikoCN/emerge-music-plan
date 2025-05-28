import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: button
    implicitWidth: 100
    implicitHeight: 40
    onHoveredChanged: {
        if(button.hovered){
            hoveringAnimation.start()
        }
        else{
            hoveredAnimation.start()
        }
    }

    property bool cache: true
    property int radius: 10
    property int iconWidth: 20
    property int iconHeight: 20
    property double hover: 0.5
    property double normal: 0
    property double borderWidth: 1
    property string iconSource: ""
    property string text: "button"
    property TikoTextLine autoText: textShow
    property bool useAutoColor: true
    property color autoColor: TikoSeit.transparentColor

    //背景颜色
    Rectangle{
        id: backRect
        color: TikoSeit.themeColor
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

    //显示图标
    Image{
        id: iconImg
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
        source: button.iconSource
        sourceSize.width: button.iconWidth
        sourceSize.height: button.iconHeight
        width: button.iconWidth
        height: button.iconHeight
        cache: button.cache

        MultiEffect {
            id: iconShow
            anchors.fill: iconImg
            source: iconImg
            colorization: useAutoColor ? 1 : 0
            colorizationColor: autoColor
        }
    }

    //显示文本
    TikoTextLine{
        id: textShow
        anchors.left: iconImg.right
        anchors.leftMargin: 3
        text: button.text
        width: button.width - iconImg.width - 20
        height: button.height
        verticalAlignment: Text.AlignVCenter
    }
}
