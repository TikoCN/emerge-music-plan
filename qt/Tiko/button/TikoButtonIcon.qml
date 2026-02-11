import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: iconButton
    implicitWidth: 32
    implicitHeight: 32
    icon: iconItem

    dynamicState.onIsHoverChanged: {
        if(iconButton.dynamicState.isHover){
           hoverAnim.start()
        }
    }

    TikoDynamicIcon {
        id: iconItem
        anchors.fill: parent
        dynamicState: iconButton.dynamicState
    }

    SequentialAnimation {
        id: hoverAnim
        property int r: 20

        RotationAnimation{
            target: iconItem
            from: 0
            to: hoverAnim.r
            duration: 100
        }


        RotationAnimation{
            target: iconItem
            from: hoverAnim.r
            to: -hoverAnim.r
            duration: 200
        }

        RotationAnimation{
            target: iconItem
            from: -hoverAnim.r
            to: 0
            duration: 100
        }
    }
}
