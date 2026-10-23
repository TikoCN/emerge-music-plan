import QtQuick
import QtQuick.Window
import Tiko
import PlayView
import MediaerAPI

GridView {
    id: gridItem

    property int row: 1
    property int column: 6
    property int realCellWidth: gridItem.cellWidth - TikoSeit.emphasizeMargins
    property real settledWidth: width
    property bool autoHeightEnable: true
    property var ownerWindow: Window.window
    property bool resizing: ownerWindow !== null && ownerWindow.sizeChange

    height: autoHeightEnable ? (cellHeight + TikoSeit.emphasizeMargins) * Math.max(row, 1) : (parent ? parent.height : 0)
    flow: GridView.FlowTopToBottom
    flickableDirection: flow === GridView.FlowLeftToRight ? Flickable.VerticalFlick : Flickable.HorizontalFlick
    snapMode: GridView.NoSnap
    highlightRangeMode: GridView.NoHighlightRange
    boundsBehavior: Flickable.StopAtBounds
    clip: true
    bottomMargin: 120
    currentIndex: 0
    reuseItems: true
    cellWidth: settledWidth / column
    // 行高由封面宽度、两行文字和悬停留白确定，避免卡片初始化反复触发网格重排。
    cellHeight: Math.max(0, realCellWidth - TikoSeit.subitemSpace * 4) + 70
    cacheBuffer: cellHeight * 2

    onWidthChanged: resizeTimer.restart()

    Component.onCompleted: resizeTimer.restart()

    Connections {
        target: gridItem.ownerWindow
        function onSizeChangeChanged() {
            if (!gridItem.resizing)
                resizeTimer.restart()
        }
    }

    Timer {
        id: resizeTimer
        interval: 180
        repeat: false
        onTriggered: {
            if (gridItem.resizing)
                return

            gridItem.settledWidth = gridItem.width
        }
    }

    delegate: ButtonAlbum {
        width: realCellWidth
        height: gridItem.cellHeight
        property bool pooled: false
        scrolling: gridItem.moving || gridItem.resizing || pooled
        GridView.onPooled: pooled = true
        GridView.onReused: pooled = false
    }
}
