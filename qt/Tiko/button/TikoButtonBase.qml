import QtQuick

Item {
    id: buttonBase

    property bool hovered: mouseArea.containsMouse
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
}
