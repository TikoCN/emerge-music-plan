import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {

    TikoTextLine {
        id: randMusicText
        exSize: 9
        opacity: 0.8
        width: parent.width
        text: qsTr("随机推荐歌曲")
    }

    ListView {
        id: randMusicList
        anchors.top: randMusicText.bottom
        anchors.topMargin: 6
        width: parent.width
        height: CoreData.recomLineHeight
        orientation: ListView.Horizontal
        spacing: CoreData.cellItemSpace
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel {
            id: musicRandModel
        }

        delegate: CoreMusicButton {
            musicId: inMusicId
            onPlayMusic: MediaPlayer.buildPlayingListByMusicId(musicId)
        }
    }

    function build () {
        var musicList = SQLData.getMusicRandList()
        for (var i in musicList) {
            musicRandModel.append({inMusicId: musicList[i]})
        }
    }

    function clear(){
        musicRandModel.clear()
    }
}
