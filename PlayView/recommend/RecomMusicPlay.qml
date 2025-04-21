import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    height: childrenRect.height

    TikoTextLine {
        id: playMoreMusicText
        exSize: 9
        opacity: 0.7
        width: parent.width
        text: qsTr("最爱的歌曲")
    }

    GridView {
        id: playMoreMusicGrid
        anchors.top: playMoreMusicText.bottom
        anchors.topMargin: 6
        height: 270
        width: parent.width - 10
        flow: GridView.FlowTopToBottom
        cellWidth: width / 3
        cellHeight: 90
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel{
            id: playMoreMusicModel
        }

        delegate: CoreMusicLine {
            width: playMoreMusicGrid.width / 3 - 10
            musicId: inMusicId
            isLittle: true
            onPlay: MediaPlayer.buildMusic(inMusicId)
        }
    }

    function build(){
        var playMoreMusicList = SQLData.getReadMoreList()
        for (var i in playMoreMusicList) {
            playMoreMusicModel.append({inMusicId: playMoreMusicList[i]})
        }
    }

    function clear(){
        playMoreMusicModel.clear()
    }
}
