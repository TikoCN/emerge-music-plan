import QtQuick
import Tiko
import PlayView
import MediaerAPI

GridView {
    id: gridItem

    property int row: 1                        // 可见行数
    property int column: 6                    // 列数
    property int pageSize: 30                // 每页加载数量
    property int realCellWidth: gridItem.cellWidth - TikoSeit.emphasizeMargins
    property bool loadEnable: false
    property bool loadIsFinish: false
    property bool autoHeightEnable: true

    property int _loadPos: 0                 // 当前已加载位置
    property bool _loadingMore: false       // 是否正在加载

    signal initData()
    signal loadData(int index, int pageSize)

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
        if (loadEnable && !loadIsFinish && atYEnd && flow === GridView.LeftToRight) loadMore()
    }

    onAtXEndChanged: {
        if (loadEnable && !loadIsFinish && atXEnd && flow === GridView.TopToBottom) loadMore()
    }

    function setGridHeight(cell) {
        if (gridItem.cellHeight !== cell.height) {
            gridItem.cellHeight = cell.height
            if (autoHeightEnable)
                gridItem.height = (gridItem.cellHeight + TikoSeit.emphasizeMargins) * Math.max(row, 1)
        }
    }

    function loadMore() {
        if(_loadingMore) return

        _loadingMore = true

        loadData(_loadPos, pageSize)
        _loadPos += pageSize

        _loadingMore = false
    }

    function clear() {
        _loadPos = 0
        gridModelItem.clear()
    }

    function build(){
        clear()
        initData()
    }

    Component.onCompleted: build()

    function toModel(list) {
        list.forEach(id => {gridModelItem.append({id: id})})
    }
}
