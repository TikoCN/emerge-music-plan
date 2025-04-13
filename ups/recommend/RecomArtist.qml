import QtQuick
import Tiko
import MediaerAPI
import Ups

Item {
    width: showColumn.width
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
        height: width / 6 - spacing + Setting.mainFont.pixelSize * 6
        width: parent.width
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel{
            id: artistRandModel
        }

        delegate: CoreArtistButton {
            artist: artistData
            r: artistRandList.width / 6 - artistRandList.spacing - space * 2
        }
    }

    function build(){
        var artistList = Core.getArtistRandList()
        for (var i in artistList) {
            artistRandModel.append({artistData: artistList[i]})
        }
    }

    function clear(){
        artistRandModel.clear()
    }
}
