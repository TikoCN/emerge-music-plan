import QtQuick
import QtQuick.Controls.Basic
import Tiko

// Horizontal 水平
Slider {
    id: hSlider
    padding: 0
    orientation: Qt.Horizontal
    width: 100
    height: 30

    property double radius: showSpace.height * hSlider.size
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
            color: lineColor
        }
    }

    handle: Rectangle{
        id: handleRect
        color: handleColor
        border.width: 1
        width: handleCell
        height: handleCell
        radius: handleCell
        x: hSlider.visualPosition * hSlider.width - width / 2
        anchors.verticalCenter: hSlider.verticalCenter

        property double zoomIn: 0
        property double handleCell: hSlider.height * handleRect.zoomIn
    }

    states: [
        State {
            name: "active"
            when: hSlider.hovered
            PropertyChanges {
                target: handle
                zoomIn: hSlider.maxZoomIn
            }
        },
        State {
            name: "inactive"
            when: !hSlider.hovered
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
