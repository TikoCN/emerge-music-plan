import QtQuick
import Tiko

Item {
    id: baseButton
    signal rightClicked()
    signal leftClicked()
    signal clicked()

    property bool check: false
    property TikoDynamicIcon icon: null
    property TikoDynamicTextLine textLine: null
    property TikoDynamicBackGround background: null
    property TikoDynamicState dynamicState: TikoDynamicState {
        isHover: mouseArea.containsMouse && !isDisabled
        isPressed: mouseArea.containsPress && !isDisabled
        isNormal: !mouseArea.containsMouse && !isDisabled
        isDisabled: false
    }

    MouseArea{
        id: mouseArea
        anchors.fill: baseButton

        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: (mouse)=>{
                       check = !check
                       baseButton.clicked()
                       switch(mouse.button){
                           case Qt.LeftButton:
                           baseButton.leftClicked()
                           break
                           case Qt.RightButton:
                           baseButton.rightClicked()
                           break
                       }
                   }
    }

}
