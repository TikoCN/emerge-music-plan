import QtQuick
import Tiko

Item {
    id: buttonBase
    height: TikoSeit.normalLineHeight
    property color showColor: Qt.red
    property bool isPressed: mouseArea.containsPress
    property bool isHover: mouseArea.containsMouse
    property bool isDisable: false
    property var buttonStates: ({
         NORMAL: "normal",
         HOVER: "hover",
         PRESSED: "pressed",
         DISABLE: "disable"
     })

    signal clickLeft()
    signal clickRight()
    signal click()

    MouseArea{
        id: mouseArea
        hoverEnabled: true
        anchors.fill: buttonBase
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse)=>{
                       click()
                       switch(mouse.button){
                           case Qt.LeftButton:
                           clickLeft()
                           break
                           case Qt.RightButton:
                           clickRight()
                           break
                       }
                   }
    }

    states: [
        State {
            name: buttonStates.HOVER
            when: mouseArea.containsMouse && !isDisable && isPressed
            PropertyChanges { target: buttonBase; showColor: TikoSeit.buttonHoverColor }
        },
        State {
            name: buttonStates.NORMAL
            when: !mouseArea.containsMouse && !isDisable && isPressed
            PropertyChanges { target: buttonBase; showColor: TikoSeit.buttonNormalColor }
        },
        State {
            name: buttonStates.PRESSED
            when: isPressed && !isDisable
           PropertyChanges { target: buttonBase; showColor: TikoSeit.buttonPressedColor }
        },
        State {
            name: buttonStates.DISABLE
            when: isDisable
           PropertyChanges { target: buttonBase; showColor: TikoSeit.buttonDisabledColor }
        }
    ]
}
