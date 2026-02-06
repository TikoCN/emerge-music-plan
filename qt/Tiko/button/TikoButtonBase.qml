import QtQuick
import Tiko

Item {
    id: buttonBase
    implicitHeight: 50
    implicitWidth: 100

    property color iconShowColor: TikoSeit.textShowButtonColor
    property color iconHoverButtonColor: TikoSeit.textHoverButtonColor
    property color iconNormalButtonColor: TikoSeit.textNormalButtonColor
    property color iconPressedButtonColor: TikoSeit.textPressedButtonColor
    property color iconDisabledButtonColor: TikoSeit.textDisabledButtonColor

    property color textShowButtonColor: TikoSeit.textShowButtonColor
    property color textHoverButtonColor: TikoSeit.textHoverButtonColor
    property color textNormalButtonColor: TikoSeit.textNormalButtonColor
    property color textPressedButtonColor: TikoSeit.textPressedButtonColor
    property color textDisabledButtonColor: TikoSeit.textDisabledButtonColor

    property color backgroundShowButtonColor: TikoSeit.backgroundNormalButtonColor
    property color backgroundHoverButtonColor: TikoSeit.backgroundHoverButtonColor
    property color backgroundNormalButtonColor: TikoSeit.backgroundNormalButtonColor
    property color backgroundPressedButtonColor: TikoSeit.backgroundPressedButtonColor
    property color backgroundDisabledButtonColor: TikoSeit.backgroundDisabledButtonColor

    property color borderShowButtonColor: TikoSeit.borderShowButtonColor
    property color borderHoverButtonColor: TikoSeit.borderHoverButtonColor
    property color borderNormalButtonColor: TikoSeit.borderNormalButtonColor
    property color borderPressedButtonColor: TikoSeit.borderPressedButtonColor
    property color borderDisabledButtonColor: TikoSeit.borderDisabledButtonColor

    property bool isPressed: false
    property bool isHover: mouseArea.containsMouse
    property bool isDisable: false

    property bool isTextDynamicColor: false
    property bool isIconDynamicColor: false
    property bool isBackgroundDynamicColor: false
    property bool isBorderColor: false

    property bool isTextShow: false
    property bool isIconShow: false
    property bool isBackgroundShow: false
    property bool isBorderShow: false
    property bool isDrawShow: false

    property var buttonStates: ({
         NORMAL: "normal",
         HOVER: "hover",
         PRESSED: "pressed",
         DISABLE: "disable"
     })

    signal rightClicked()
    signal leftClicked()
    signal clicked()

    MouseArea{
        id: mouseArea
        hoverEnabled: true
        anchors.fill: buttonBase
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse)=>{
                       buttonBase.clicked()
                       switch(mouse.button){
                           case Qt.LeftButton:
                           buttonBase.leftClicked()
                           break
                           case Qt.RightButton:
                           buttonBase.rightClicked()
                           break
                       }
                   }
    }

    states: [
        State {
            name: buttonStates.HOVER
            when: isHover && !isDisable && !isPressed
            PropertyChanges {
                target: buttonBase
                iconShowColor: iconHoverColor
                textShowColor: textHoverColor
                backgroundShowColor: backgroundHoverColor
                borderShowColor: borderHoverColor
            }
        },
        State {
            name: buttonStates.NORMAL
            when: !isHover && !isDisable && !isPressed
            PropertyChanges {
                target: buttonBase
                iconShowColor: iconNormalColor
                textShowColor: textNormalColor
                backgroundShowColor: backgroundNormalColor
                borderShowColor: borderNormalColor
            }
        },
        State {
            name: buttonStates.PRESSED
            when: isPressed && !isDisable
            PropertyChanges {
                target: buttonBase
                iconShowColor: iconPressedColor
                textShowColor: textPressedColor
                backgroundShowColor: backgroundPressedColor
                borderShowColor: borderPressedColor
            }
        },
        State {
            name: buttonStates.DISABLE
            when: isDisable
            PropertyChanges {
                target: buttonBase
                iconShowColor: iconDisableColor
                textShowColor: textDisableColor
                backgroundShowColor: backgroundDisableColor
                borderShowColor: borderDisableColor
            }
        }
    ]

    Rectangle{
        anchors.fill: parent
        z: -1
        color: backgroundShowColor
        border.color: borderShowColor
        radius: 8
    }

    TikoImage {
        id: iconItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.normalMargins
        anchors.horizontalCenter: parent.horizontalCenter
        autoColor: iconShowColor
        visible: isIconShow
    }

    Item {
        id: drawItem
        anchors.left: iconItem.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.horizontalCenter: parent.horizontalCenter
        visible: isDrawShow
    }

    TikoTextLine {
        id: textLineItem
        anchors.left: drawItem.left
        anchors.leftMargin: TikoSeit.subitemSpace
        anchors.horizontalCenter: parent.horizontalCenter
        autoColor: textShowColor
        visible: isTextShow
    }
}
