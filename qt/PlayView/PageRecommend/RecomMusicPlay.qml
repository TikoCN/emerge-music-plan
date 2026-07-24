import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    height: childrenRect.height

    TikoTextLine {
        id: playMoreMusicText
        opacity: 0.7
        width: parent.width
        text: qsTr("最爱的歌曲")
        level: 2
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

        model: MusicLibrary.model()

        delegate: CoreMusicLine {
            width: playMoreMusicGrid.width / 3 - 10
            isLittle: true
            onPlayMusic: MediaPlayer.buildPlayingListByMusicId(model.id)
        }
    }

    function build(){
        MusicLibrary.model().loadMostPlayedList()
    }

    function clear(){
        MusicLibrary.model().clear()
    }
}