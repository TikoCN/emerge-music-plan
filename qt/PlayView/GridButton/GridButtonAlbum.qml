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
    property LoadBase dataLoader: LoadBase{}
    property string currentKey: ""

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
                dataLoader.loadMore()
            }
        }
    }

    onAtYEndChanged: {
        if (flow === 0) {
            if (atYEnd) {
                dataLoader.loadMore()
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
        dataLoader.reset()
    }

    function loadByKey(key, size, offset) {
        AlbumLibrary.model().loadByKey(key, size, offset)
    }

    function loadMoreByKey(key, size, offset) {
        AlbumLibrary.model().loadMoreByKey(key, size, offset)
    }
}