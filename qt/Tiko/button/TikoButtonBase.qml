import QtQuick
import Tiko

MouseArea {
    id: mouseArea
    hoverEnabled: true
    acceptedButtons: Qt.LeftButton | Qt.RightButton
    onClicked: (mouse)=>{
                   check = !check
                   anyClicked()

                   if (propagateComposedEvents) {
                       mouse.accepted = false
                   }

                   switch(mouse.button){
                       case Qt.LeftButton:
                       leftClicked()
                       break
                       case Qt.RightButton:
                       rightClicked()
                       break
                   }
               }

    signal rightClicked()
    signal leftClicked()
    signal anyClicked()

    property bool check: false
    property TikoDynamicIcon icon: null
    property TikoDynamicTextLine textLine: null
    property TikoDynamicBackGround background: null
    property TikoDynamicState dynamicState: TikoDynamicState {
        isHover: containsMouse
        isPressed: containsPress
        isNormal: !containsMouse
        isDisabled: false
    }
}
