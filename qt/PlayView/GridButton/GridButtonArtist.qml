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

    model: ArtistLibrary.model()

    delegate: ButtonArtist {
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }

    onAtXEndChanged: {
        if (flow === 1) {
            if (atXEnd) {
                ArtistLibrary.loader().loadMore()
            }
        }
    }

    onAtYEndChanged: {
        if (flow === 0) {
            if (atYEnd) {
                ArtistLibrary.loader().loadMore()
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
        ArtistLibrary.loader().reset()
        resetRequested()
    }

    function loadByKey(key) {
        ArtistLibrary.loader().setCurrentKey(key)
        ArtistLibrary.loader().reset()
    }
}