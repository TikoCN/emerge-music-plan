import QtQuick
import Tiko

Item {
    id: buttonBase
    signal rightClicked()
    signal leftClicked()
    signal clicked()

    property TikoDynamicIcon icon: null
    property TikoDynamicTextLine textLine: null
    property TikoDynamicBackGround background: null
    property TikoDynamicState dynamicState: TikoDynamicState {
        isHover: mouseArea.containsMouse && !isDisabled
        isPressed: mouseArea.containsPress && !isDisabled
        isNormal: !mouseArea.containsMouse && !isDisabled
        isDisabled: false
    }

    property bool check: false

    MouseArea{
        id: mouseArea
        hoverEnabled: true
        anchors.fill: buttonBase
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse)=>{
                       check = !check
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
}
