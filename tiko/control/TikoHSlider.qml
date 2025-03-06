import QtQuick
import QtQuick.Controls.Basic
import "../"

// Horizontal 水平
Slider {
    id: hSlider
    padding: 0
    orientation: Qt.Horizontal
    onHoveredChanged: {
        if(hSlider.hovered){
            hoverAnimation.start()
        }
        else{
            closeAnimation.start()
        }
    }

    property double radius: 0
    property color lineColor: TikoSeit.transparentColor
    property color showColor: TikoSeit.themeColor
    property color handleColor: TikoSeit.backdropColor
    property double size: 0.2

    //滑行航道
    background: Item{
        id: showSpace
        anchors.fill: parent

        //背景条
        Rectangle{
            color: hSlider.lineColor
            opacity: 0.1
            radius: hSlider.radius
            anchors.verticalCenter: showSpace.verticalCenter
            x: 0
            width: showSpace.width
            height: showSpace.height * hSlider.size
        }

        //划过的航道
        Rectangle{
            radius: hSlider.radius
            width: hSlider.visualPosition * showSpace.width
            height: showSpace.height * hSlider.size
            anchors.verticalCenter: showSpace.verticalCenter
            x: 0
            color: hSlider.showColor
            opacity: 0.9
        }
    }

    handle: Rectangle{
        id: handleRect
        color: hSlider.handleColor
        border.color: hSlider.showColor
        border.width: 1
        width: hSlider.height * handleRect.zoomIn
        height: hSlider.height * handleRect.zoomIn
        radius: hSlider.height * handleRect.zoomIn
        x: hSlider.visualPosition * hSlider.width - width / 2
        anchors.verticalCenter: hSlider.verticalCenter

        property double zoomIn: 0
    }


    //游览动画
    ParallelAnimation{
        id: hoverAnimation

        NumberAnimation{
            target: handleRect
            easing.type: Easing.OutElastic
            property: "zoomIn"
            from: 0
            to: 1 + hSlider.size
            duration: 1000
        }
    }

    //关闭动画
    ParallelAnimation{
        id: closeAnimation

        NumberAnimation{
            target: handleRect
            easing.type: Easing.InElastic
            property: "zoomIn"
            from: 1 + hSlider.size
            to: 0
            duration: 1000
        }
    }
}
