import QtQuick
import Tiko

Item{
    id: alumbButton
    width: 150
    height: alumbCover.height + textLine.height + 10

    property var alumb
    property string name: alumb.name
    property int coverId: alumb.musicList[0].coreId

    TikoImageAuto {
        id: alumbCover
        width: parent.width
        height: parent.width
        normalUrl: "qrc:/image/alumb.png"
        loadUrl: "image://cover/file:" + alumbButton.coverId.toString()
    }

    TikoTextLine {
        id: textLine
        anchors.top: alumbCover.bottom
        anchors.topMargin: 10
        width: parent.width
        text: alumbButton.name
    }

    MouseArea {
        anchors.fill: alumbCover
        onClicked: mainView.turnToAlumbPlayer(alumb)
    }
}
