import QtQuick
import QtQuick.Controls.Basic
import Tiko
TikoButtonBase {
    id: switchButton

    implicitHeight: 36
    textLine: textLineItem
    property color useingColor: TikoSeit.theme.baseTheme.borderEmphasize
    property color unuseColor: TikoSeit.theme.baseTheme.backgroundEmphasize
    property double fontOpacity: 0.3

    onCheckChanged: {
        colorAnimation.start()
        propertyAnimation.start()
    }

    Rectangle{
        id: box
        color: switchButton.check ? switchButton.useingColor : switchButton.unuseColor
        border.color: !switchButton.check ? switchButton.useingColor : switchButton.unuseColor
        height: switchButton.height * 0.6
        width: height * 1.7
        anchors.verticalCenter: switchButton.verticalCenter
        radius: height * 0.5
        anchors.left: switchButton.left
        property double spacer: box.height * 0.1

        Rectangle{
            id: show
            x: check ? box.width - show.width - box.spacer: box.spacer
            y: box.spacer
            width: height
            height: box.height - box.spacer * 2
            color: TikoSeit.theme.baseTheme.themeTransition
            radius: height * 0.5
        }

        PropertyAnimation{
            id: propertyAnimation
            target: show
            property: "x"
            from: !switchButton.check ? box.width - show.width - box.spacer: box.spacer
            to: switchButton.check ? box.width - show.width - box.spacer: box.spacer
            duration: 500
        }

        ColorAnimation {
            id: colorAnimation
            target: box
            property: "color"
            from: !switchButton.check ? switchButton.useingColor : switchButton.unuseColor
            to: switchButton.check ? switchButton.useingColor : switchButton.unuseColor
            duration: 500
        }


    }

    TikoDynamicTextLine{
        id: textLineItem
        anchors.left: box.right
        anchors.leftMargin: 10
        height: switchButton.height
        width: switchButton.width - box.width - 20
        opacity: check ? 1 : fontOpacity
    }
}
