import QtQuick
import PlayView
QtObject {
    property int loadPos: 0                 // 当前已加载位置
    property bool loadingMore: false       // 是否正在加载

    property bool loadEnable: true
    property bool loadIsFinish: false
    property bool loadInit: false

    signal loadData(int index)
    signal loadInitData()

    function loadMore() {
        if(!loadEnable || loadIsFinish || !loadInit || loadingMore ) return

        loadingMore = true

        loadData(loadPos)

        loadingMore = false
    }

    function reset() {
        loadPos = 0
        loadInit = false
        loadIsFinish = false
        init()
    }

    function init () {
        loadInitData()
        loadInit = true
    }
}
