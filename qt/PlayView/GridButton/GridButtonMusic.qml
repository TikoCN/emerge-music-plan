import QtQuick
import Tiko
import PlayView
import MediaerAPI

GridView {
    id: gridItem

    property int row: 1
    property int column: 6
    property int realCellWidth: gridItem.cellWidth - TikoSeit.emphasizeMargins
    property bool autoHeightEnable: true
    property string currentKey: ""
    signal resetRequested()

    height: 50
    flow: GridView.TopToBottom
    clip: true
    currentIndex: 0
    reuseItems: true
    highlightRangeMode: ListView.StrictlyEnforceRange
    preferredHighlightBegin: 0
    preferredHighlightEnd: 0
    cellWidth: width / column
    cellHeight: 100

    model: MusicLibrary.model()

    delegate: ButtonMusic {
        width: realCellWidth
        onPlayMusic: MediaPlayer.buildPlayingList([model.id])
        onHeightChanged: setGridHeight(this)
    }

    onAtXEndChanged: {
        if (flow === 1) {
            if (atXEnd) {
                MusicLibrary.loader().loadMore()
            }
        }
    }

    onAtYEndChanged: {
        if (flow === 0) {
            if (atYEnd) {
                MusicLibrary.loader().loadMore()
            }
        }
    }

    function setGridHeight(cell) {
        if (gridItem.cellHeight !== cell.height) {
            gridItem.cellHeight = cell.height
            if (autoHeightEnable)
                gridItem.height = (gridItem.cellHeight + TikoSeit.emphasizeMargins) * Math.max(row, 1)
        }
    }

    function reset() {
        MusicLibrary.loader().reset()
        resetRequested()
    }

    function loadByKey(key) {
        MusicLibrary.loader().setCurrentKey(key)
        MusicLibrary.loader().reset()
    }
}