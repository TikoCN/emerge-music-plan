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

    model: AlbumLibrary.model()

    delegate: ButtonAlbum {
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }

    onAtXEndChanged: {
        if (flow === 1) {
            if (atXEnd) {
                AlbumLibrary.loader().loadMore()
            }
        }
    }

    onAtYEndChanged: {
        if (flow === 0) {
            if (atYEnd) {
                AlbumLibrary.loader().loadMore()
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
        AlbumLibrary.loader().reset()
        resetRequested()
    }

    function loadByKey(key) {
        AlbumLibrary.loader().setCurrentKey(key)
        AlbumLibrary.loader().reset()
    }
}