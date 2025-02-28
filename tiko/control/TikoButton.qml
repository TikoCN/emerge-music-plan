import QtQuick.Controls.Basic
import QtQuick
import "../"

Button {
    id: button
    implicitWidth: 100
    implicitHeight: 40
    icon.width: 20
    icon.height: 20
    text: qsTr("Tiko的按钮")
    onHoveredChanged: {
        if(button.hovered){
            hoveringAnimation.start()
        }
        else{
            hoveredAnimation.start()
        }
    }

    property int radius: 10
    property double hover: 0.5
    property double normal: 0
    property bool cache: true
    property TikoAutoText textShow: textShow
    property double borderWidth: 1

    //背景颜色
    background: Rectangle{
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

    contentItem: Item {
        Image{
            id: iconImg
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            source: button.icon.source
            sourceSize.width: button.icon.width
            sourceSize.height: button.icon.height
            width: button.icon.width
            height: button.icon.height
            cache: button.cache
        }

        TikoAutoText{
            id: textShow
            anchors.left: iconImg.right
            anchors.leftMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            text: button.text
            width: button.width - iconImg.width - 20
        }
    }
}
