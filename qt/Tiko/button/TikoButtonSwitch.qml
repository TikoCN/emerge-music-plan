import QtQuick
import QtQuick.Controls.Basic
import Tiko
TikoButtonBase {
    id: switchButton

    implicitHeight: 36
    textLine: textLineItem
    property color useingColor: TikoSeit.theme.baseTheme.borderEmphasize
    property color unuseColor: TikoSeit.theme.baseTheme.backgroundEmphasize
    property color useingShowColor: TikoSeit.theme.baseTheme.themeNormal
    property color unuseShowColor: TikoSeit.theme.baseTheme.foregroundEmphasize
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
        color: check ? useingColor : unuseColor
        border.color: !check ? useingColor : unuseColor
        height: switchButton.height * 0.6
        width: height * 1.7
        anchors.verticalCenter: switchButton.verticalCenter
        radius: height * 0.5
        anchors.left: switchButton.left
        property double spacer: box.height * 0.1

        Rectangle{
            id: show
            x: check ? useingX: unuseX
            y: box.spacer
            width: height
            height: box.height - box.spacer * 2
            color: check ? useingShowColor : unuseShowColor
            radius: height * 0.5
        }

        ParallelAnimation {
            id: activeTrueAnim
            PropertyAnimation{
                target: show
                property: "x"
                from: unuseX
                to: useingX
                duration: 500
            }
            PropertyAnimation{
                target: show
                property: "color"
                from: unuseShowColor
                to: useingShowColor
                duration: 500
            }
            ColorAnimation {
                target: box
                property: "color"
                from: unuseColor
                to: useingColor
                duration: 500
            }
        }

        ParallelAnimation {
            id: activeFalseAnim
            PropertyAnimation{
                target: show
                property: "x"
                from: useingX
                to: unuseX
                duration: 500
            }
            PropertyAnimation{
                target: show
                property: "color"
                from: useingShowColor
                to: unuseShowColor
                duration: 500
            }
            ColorAnimation {
                target: box
                property: "color"
                from: useingColor
                to: unuseColor
                duration: 500
            }
        }
    }

    TikoDynamicTextLine{
        id: textLineItem
        anchors.left: box.right
        anchors.leftMargin: 10
        height: switchButton.height
        width: switchButton.width - box.width - TikoSeit.emphasizeMargins
        opacity: check ? 1 : fontOpacity
    }
}
