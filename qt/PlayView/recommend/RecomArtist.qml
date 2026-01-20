import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    height: childrenRect.height

    TikoTextLine {
        exSize: 9
        opacity: 0.7
        id: artistRandText
        width: parent.width
        text: qsTr("随机推荐歌手")
    }

    ListView {
        id: artistRandList
        anchors.top: artistRandText.bottom
        anchors.topMargin: 6
        height: CoreData.recomLineHeight
        width: parent.width
        orientation: ListView.Horizontal
        spacing: CoreData.cellItemSpace
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel{
            id: artistRandModel
        }

        delegate: CoreArtistButton {
            artistId: inArtistId
        }
    }

    function build(){
        var artistList = SQLData.getArtistRandList()
        for (var i in artistList) {
            artistRandModel.append({inArtistId: artistList[i]})
        }
    }

    function clear(){
        artistRandModel.clear()
    }
}
