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
    property bool isHover: mouseArea.containsMouse
    property bool isPress: mouseArea.containsPress
    property double bgOpacity: 0

    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.colorBgDefault
        radius: 5
        opacity: mouseArea.bgOpacity
    }
    Rectangle {
        id: tspItem
        anchors.fill: parent
        color: TikoSeit.theme.colorMaxTop
        radius: 5
        opacity: {
            if (isPress) return 0.12
        else if (isHover) return 0.08
        return 0
        }
    }
}
