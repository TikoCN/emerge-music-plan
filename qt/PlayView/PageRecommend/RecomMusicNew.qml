import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    height: childrenRect.height

    TikoTextLine {
        id: newMusicText
        opacity: 0.7
        width: parent.width
        text: qsTr("新加入歌曲")
        level: 2
    }

    GridView {
        id: newMusicGrid
        anchors.top: newMusicText.bottom
        anchors.topMargin: 6
        height: 270
        width: parent.width
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
            width: newMusicGrid.width / 3 - 10
            isLittle: true
            onPlayMusic: MediaPlayer.buildPlayingList([model.id])
        }
    }

    function build(){
        MusicLibrary.model().loadMostNewList()
    }

    function clear(){
        MusicLibrary.model().clear()
    }
}