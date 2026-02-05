import QtQuick
import QtQuick.Controls.Basic
import Tiko

TikoButtonComplete {
    id: button
    implicitWidth: textLine.font.pixelSize * 1.3 * 5
    isUseShowColor: true

    property string text: qsTr("TikoButtonBorder")
    property int radius: 20

    onIsHoverChanged : {
        if (isHover) {
                if (!inAnimation.running) {
                    outAnimation.stop();  // 停止反向动画，保证动画流畅
                    inAnimation.start();
                }
            } else {
                if (!outAnimation.running) {
                    inAnimation.stop();  // 停止正向动画，保证动画流畅
                    outAnimation.start();
                }
            }
    }

    Rectangle {
        id: back
        radius: button.radius
        border.color: showColor
        opacity: 0.3
        anchors.fill: parent

        PropertyAnimation on radius {
            id: inAnimation
            from: 0
            to: button.radius
            duration: 500
        }

        PropertyAnimation on radius {
            id: outAnimation
            from: button.radius
            to: 0
            duration: 500
        }
    }
}
