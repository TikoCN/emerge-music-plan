import QtQuick
import Tiko

Item{
    id: artistButton
    width: 150
    height: artistCover.height + textLine.height + 10

    property var artist
    property string name: artist.name
    property int coverId: artist.musicList[0].coreId

    TikoImageAuto {
        id: artistCover
        width: parent.width
        height: parent.width
        normalUrl: "qrc:/image/artist.png"
        loadUrl: "image://cover/file:" + artistButton.coverId.toString()
    }

    TikoTextLine {
        id: textLine
        anchors.top: artistCover.bottom
        anchors.topMargin: 10
        width: parent.width
        text: artistButton.name
    }

    MouseArea {
        anchors.fill: artistCover
        onClicked: mainView.openartistData(artist)
    }
}
