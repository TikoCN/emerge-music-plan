import QtQuick
import Tiko
import Ups
import MediaerAPI

Item {

    TikoTextLine {
        id: albumRandText
        exSize: 9
        opacity: 0.7
        width: parent.width
        text: qsTr("随机推荐专辑")
    }

    ListView {
        id: albumRandList
        anchors.top: albumRandText.bottom
        anchors.topMargin: 6
        height: albumRandList.width / 6 - albumRandList.spacing - 10 + Setting.mainFont.pixelSize * 6
        width: parent.width
        orientation: ListView.Horizontal
        spacing: 10
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel{
            id: albumRandModel
        }

        delegate: CoreAlbumButton {
            album: albumData
            r: albumRandList.width / 6 - albumRandList.spacing - space * 2
        }
    }


    function build(){
        var albumList = Core.getAlbumRandList()
        for (var i in albumList) {
            albumRandModel.append({albumData: albumList[i]})
        }
    }
}
