import QtQuick
import Tiko
import MediaerAPI
import Ups

Item {
    width: showColumn.width
    height: childrenRect.height

    TikoTextLine {
        id: newMusicText
        exSize: 9
        opacity: 0.7
        width: parent.width
        text: qsTr("新加入歌曲")
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

        model: ListModel{
            id: newMusicModel
        }

        delegate: CoreMusicLine {
            width: newMusicGrid.width / 3 - 10
            music: musicData
            isLittle: true
            type: -1
        }
    }

    function build(){
        var newMusicList = Core.getNewMusicList()
        for (var i in newMusicList) {
            newMusicModel.append({musicData: newMusicList[i]})
        }
    }

    function clear(){
        newMusicModel.clear()
    }
}
