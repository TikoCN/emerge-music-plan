import QtQuick

Item {
    id: buttonBase

    property bool hovered: mouseArea.containsMouse
    signal clickLeft()
    signal clickRight()

    MouseArea{
        id: mouseArea
        hoverEnabled: true
        anchors.fill: buttonBase
        onClicked: (mouse)=>{
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
