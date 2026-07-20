import QtQuick
import QtQuick.Controls.Basic
import Tiko
TikoButtonBase {
    id: switchButton

    width: 200
    height: 50
    property TikoTextLine textLine: textLineItem
    property int useingX: box.width - show.width - box.spacer
    property int unuseX: box.spacer
    property double fontOpacity: 0.3

    onCheckChanged: {
        activeTrueAnim.stop()
        activeFalseAnim.stop()

        if (check) {
            activeTrueAnim.start()
        } else {
            activeFalseAnim.start()
        }
    }

    Rectangle{
        id: box
        color: TikoSeit.theme.colorBgDefault
        height: switchButton.height * 0.6
        width: height * 1.7
        anchors.verticalCenter: switchButton.verticalCenter
        radius: height * 0.5
        anchors.left: switchButton.left
        property double spacer: box.height * 0.1

        // 滑块
        Rectangle{
            id: show
            x: unuseX
            y: box.spacer
            width: height
            height: box.height - box.spacer * 2
            color: TikoSeit.theme.colorFgDefault
            radius: height * 0.5
        }

        ParallelAnimation {
            id: activeTrueAnim
            PropertyAnimation{
                target: show
                property: "x"
                from: unuseX
                to: useingX
                duration: 250
            }
        }

        ParallelAnimation {
            id: activeFalseAnim
            PropertyAnimation{
                target: show
                property: "x"
                from: useingX
                to: unuseX
                duration: 250
            }        
        }
    }

    Rectangle {
        anchors.fill: box
        color: Qt.rgba(1,1,1,1)
        radius: height * 0.5
        opacity: {
                   if (switchButton.isPress) return 0.16   // 按下：12% 白
                   if (switchButton.isHover) return 0.12   // 悬停：8% 白
                   return 0.08
               }
    }

    TikoTextLine{
        id: textLineItem
        anchors.left: box.right
        anchors.leftMargin: 10
        height: switchButton.height
        width: switchButton.width - box.width - TikoSeit.emphasizeMargins
        opacity: check ? 1 : fontOpacity
        text: "TikoButtonSwitch"
    }
}
