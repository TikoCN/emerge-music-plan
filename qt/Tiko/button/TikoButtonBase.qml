import QtQuick
import Tiko

Item {
    id: buttonBase
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
}
