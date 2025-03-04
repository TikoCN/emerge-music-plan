import QtQuick
import QtQuick.Controls.Basic

Item {
    id: switchButton

    implicitHeight: 36

    property bool check: false
    property string text: qsTr("文本")
    property color useingColor: TikoSeit.themeColor
    property color unuseColor: TikoSeit.transparentColor

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            check = !check
            colorAnimation.start()
            propertyAnimation.start()
        }
    }

    Rectangle{
        id: box
        color: switchButton.check ? switchButton.useingColor : switchButton.unuseColor
        height: switchButton.height * 0.6
        width: height * 1.7
        anchors.verticalCenter: switchButton.verticalCenter
        opacity: 0.6
        radius: height * 0.5
        anchors.right: switchButton.right
        property double spacer: box.height * 0.1

        Rectangle{
            id: show
            x: check ? box.width - show.width - box.spacer: box.spacer
            y: box.spacer
            width: height
            height: box.height - box.spacer * 2
            color: TikoSeit.backdropColor
            opacity: 1
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

    TikoAutoText{
        text: switchButton.text
        anchors.left: switchButton.left
        anchors.leftMargin: 10
        height: switchButton.height
        width: switchButton.width - box.width - 20
        color: mouseArea.containsMouse ? switchButton.useingColor : switchButton.unuseColor
    }
}
