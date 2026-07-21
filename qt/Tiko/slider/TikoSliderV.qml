import QtQuick
import QtQuick.Controls.Basic
import Tiko

// Vertical 竖直
Slider {
    id: vSlider
    padding: 0
    orientation: Qt.Vertical
    width: 30
    height: 100

    property double radius: showSpace.width * vSlider.size
    property color lineColor: TikoSeit.theme.colorFgDefault
    property color bgColor: TikoSeit.theme.colorBgDefault
    property color handleColor: TikoSeit.theme.colorHighlight
    property double size: 0.2
    property double maxZoomIn: 1.3

    //滑行航道
    background: Item{
        id: showSpace
        anchors.fill: parent

        //背景条
        Rectangle{
            color: bgColor
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
            color: lineColor
            opacity: 1
        }
    }


    //滑块
    handle: Rectangle{
        id: handleRect
        color: handleColor
        border.width: 3
        width: vSlider.width * handleRect.zoomIn
        height: vSlider.width * handleRect.zoomIn
        radius: vSlider.width * handleRect.zoomIn
        anchors.horizontalCenter: vSlider.horizontalCenter
        y: vSlider.visualPosition * vSlider.height - handleRect.height / 2

        property double zoomIn: 0
    }

    states: [
        State {
            name: "active"
            when: vSlider.hovered
            PropertyChanges {
                target: handle
                zoomIn: maxZoomIn
            }
        },
        State {
            name: "inactive"
            when: !vSlider.hovered
            PropertyChanges {
                target: handle
                zoomIn: 0
            }
        }
    ]

    transitions: Transition {
        // 同时动画 zoomIn 和 opacity
        ParallelAnimation {
            NumberAnimation {
                properties: "zoomIn"
                duration: 250
                easing.type: Easing.OutQuad
            }
        }
    }
}
