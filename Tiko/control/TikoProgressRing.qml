import QtQuick
import QtQuick.Shapes
import QtQuick.Controls
import "../"

Shape {
    id: progressRing
    width: progressRing.diameter
    height: progressRing.diameter

    property double precent: 0
    property double radius: 50
    property double diameter: progressRing.radius * 2
    property color backColor: TikoSeit.backdropColor
    property color usedColor: TikoSeit.themeColor
    property color transparentColor: TikoSeit.transparentColor

    Rectangle{
        anchors.centerIn: progressRing
        width: progressRing.diameter * 0.8
        height: progressRing.diameter * 0.8
        radius: progressRing.diameter * 0.4
        color: progressRing.backColor

        TikoTextLine {
            property int finish: progressRing.precent * 100
            anchors.centerIn: parent
            text: finish.toString()+ "%"
        }
    }

    //背景
    Rectangle{
        anchors.centerIn: progressRing
        width: progressRing.diameter
        height: progressRing.diameter
        radius: progressRing.diameter * 0.5
        color: progressRing.transparentColor
        opacity: 0.03
    }

    ShapePath{
        startX: progressRing.radius
        startY: 0

        fillGradient: ConicalGradient{
            centerX: progressRing.radius
            centerY: progressRing.radius
            GradientStop{
                position: 0.0
                color: progressRing.usedColor
            }
            GradientStop{
                position: progressRing.precent
                color: progressRing.usedColor
            }
            GradientStop{
                position: progressRing.precent + 0.001
                color: progressRing.backColor
            }
            GradientStop{
                position: 1.0001
                color: progressRing.backColor
            }
        }

        //圆形路径
        PathArc{
            x: progressRing.radius
            y: progressRing.diameter
            radiusX: progressRing.radius
            radiusY: progressRing.radius
        }
        PathArc{
            x: progressRing.radius
            y: 0
            radiusX: progressRing.radius
            radiusY: progressRing.radius
        }
    }
}
