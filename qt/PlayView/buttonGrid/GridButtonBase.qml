import QtQuick
import Tiko
import PlayView
import MediaerAPI

GridView {
    id: gridItem

    property int row: 1                        // 可见行数
    property int column: 6                    // 列数
    property int realCellWidth: gridItem.cellWidth - TikoSeit.emphasizeMargins
    property bool autoHeightEnable: true
    property LoadBase dataLoader: LoadBase{}
    property alias gridModel: gridModelItem

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

    model: ListModel{
        id: gridModelItem
    }

    onAtYEndChanged: {
        if (atYEnd && flow === GridView.LeftToRight) dataLoader.loadMore()
    }

    onAtXEndChanged: {
        if (atXEnd && flow === GridView.TopToBottom) dataLoader.loadMore()
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
        gridModelItem.clear()
    }
}
