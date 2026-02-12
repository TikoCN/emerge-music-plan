import QtQuick
import QtQuick.Controls.Basic
import Tiko

// Vertical 竖直
Slider {
    id: vSlider
    padding: 0
    orientation: Qt.Vertical
    onHoveredChanged: {
        if(vSlider.hovered){
            hoverAnimation.start()
        }
    }

    property double radius: 0
    property color lineColor: TikoSeit.theme.baseTheme.borderTransition
    property color showColor: TikoSeit.theme.baseTheme.themeTransition
    property color handleColor: TikoSeit.theme.baseTheme.backgroundTransition
    property double size: 0.3

    //滑行航道
    background: Item{
        id: showSpace
        anchors.fill: parent

        //背景条
        Rectangle{
            color: vSlider.lineColor
            opacity: 0.1
            radius: vSlider.radius
            y: 0
            anchors.horizontalCenter: showSpace.horizontalCenter
            width: showSpace.width * vSlider.size
            height: showSpace.height
        }

        //划过的航道
        Rectangle{
            radius: vSlider.radius
            width: showSpace.width * vSlider.size
            height: (1 - vSlider.visualPosition) * showSpace.height
            y: showSpace.height - height
            anchors.horizontalCenter: showSpace.horizontalCenter
            color: vSlider.showColor
            opacity: 1
        }
    }


    //滑块
    handle: Rectangle{
        id: handleRect
        color: vSlider.handleColor
        border.color: vSlider.showColor
        border.width: 3
        width: vSlider.width * handleRect.zoomIn
        height: vSlider.width * handleRect.zoomIn
        radius: vSlider.width * handleRect.zoomIn
        anchors.horizontalCenter: vSlider.horizontalCenter
        y: vSlider.visualPosition * vSlider.height - handleRect.height / 2

        property double zoomIn: 1.0
    }

    //游览动画
    ParallelAnimation{
        id: hoverAnimation
        onStopped: {
            if(vSlider.hovered){
                hoverAnimation.start()
            }
            else{
                closeAnimation.start()
            }
        }

        NumberAnimation{
            target: handleRect
            easing.type: Easing.OutElastic
            property: "zoomIn"
            from: 1.0
            to: 1.3
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
            from: 1.3
            to: 1.0
            duration: 1000
        }
    }
}
