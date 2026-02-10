import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import Tiko

TikoButtonBase {
    id: iconButton
    implicitWidth: 32
    implicitHeight: 32

    property TikoImage icon: iconImg

    onIsHoverChanged: {
        if(iconButton.isHover){
            rotAnimation.start()
        }
    }

    TikoImage {
        id: iconImg
        anchors.fill: parent
        unifiedColor: showColor
    }


    //旋转动画
    RotationAnimation{
        id: rotAnimation
        target: iconImg
        from: 0
        to: 360
        duration: 500
    }
}
