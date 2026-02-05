import QtQuick
import Tiko

Item {
    id: buttonBase
    implicitHeight: 50
    implicitWidth: 100

    property color showColor: Qt.red
    property color hoverColor: TikoSeit.buttonHoverColor
    property color normalColor: TikoSeit.buttonNormalColor
    property color pressedColor: TikoSeit.buttonPressedColor
    property color disableColor: TikoSeit.buttonDisabledColor

    property bool isPressed: false
    property bool isHover: mouseArea.containsMouse
    property bool isDisable: false

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
            PropertyChanges { target: buttonBase; showColor: hoverColor }
        },
        State {
            name: buttonStates.NORMAL
            when: !isHover && !isDisable && !isPressed
            PropertyChanges { target: buttonBase; showColor: normalColor }
        },
        State {
            name: buttonStates.PRESSED
            when: isPressed && !isDisable
           PropertyChanges { target: buttonBase; showColor: pressedColor }
        },
        State {
            name: buttonStates.DISABLE
            when: isDisable
           PropertyChanges { target: buttonBase; showColor: disableColor }
        }
    ]
}
