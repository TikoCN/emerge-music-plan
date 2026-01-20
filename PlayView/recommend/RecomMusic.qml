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
        height: randMusicList.width / 6
        orientation: ListView.Horizontal
        spacing: 20
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
<<<<<<< Updated upstream:PlayView/recommend/RecomMusic.qml
            height: randMusicList.height
            width: randMusicList.width / 6 - randMusicList.spacing
            onPlay: MediaPlayer.buildMusic(inMusicId)
=======
            onPlayMusic: MediaPlayer.buildPlayingListByMusicId(musicId)
>>>>>>> Stashed changes:qt/PlayView/recommend/RecomMusic.qml
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
