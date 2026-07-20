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
    property double bgOpacity: 1

    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.colorBgDefault
        radius: 5
        opacity: mouseArea.bgOpacity
    }
    Rectangle {
        anchors.fill: parent
        color: Qt.rgba(1,1,1,1)
        radius: 5
        opacity: {
                   if (mouseArea.isPress) return 0.12   // 按下：12% 白
                   if (mouseArea.isHover) return 0.08   // 悬停：8% 白
                   return 0
               }
    }
}
