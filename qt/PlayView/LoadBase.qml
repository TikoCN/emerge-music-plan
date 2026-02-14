import QtQuick
import PlayView
QtObject {
    property int loadPos: 0                 // 当前已加载位置
    property bool loadingMore: false       // 是否正在加载

    property bool loadEnable: false
    property bool loadIsFinish: false

    signal loadData(int index)


    function loadMore() {
        if(!loadEnable || loadIsFinish || loadingMore) return

        loadingMore = true

        loadData(loadPos)
        loadPos += CoreData.pageSize

        loadingMore = false
    }

    function reset() {
        loadPos = 0
    }
}
