import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

ListView{
    id: musicListView
    interactive: true
    spacing: 15
    clip: true
    reuseItems: true
    bottomMargin: 120
    ScrollBar.vertical: TikoBarV{}

    /**
     * @brief 加载可见区域的歌曲，布局尚未完成时至少请求首个可见位置。
     */
    function loadVisibleRange() {
        if (model === null || typeof model.loadRange !== "function")
            return

        const itemExtent = 70 + spacing
        const firstIndex = Math.floor(Math.max(0, contentY) / itemExtent)
        // 页面首次加入 StackView 前高度可能为负，仍需加载首屏数据。
        const lastIndex = Math.floor((Math.max(0, contentY) + Math.max(0, height)) / itemExtent)
        for (let position = firstIndex; position <= lastIndex; ++position)
            model.loadRange(position)
    }

    Component.onCompleted: {
        if (model !== null && typeof model.clear === "function") {
            model.clear()
            loadVisibleRange()
        }
    }

    onContentYChanged: loadVisibleRange()

    // 等待模型重置和布局更新完成后，再补齐当前可见区域的数据。
    onCountChanged: Qt.callLater(loadVisibleRange)

    onHeightChanged: Qt.callLater(loadVisibleRange)

    onVisibleChanged: {
        if (visible)
            Qt.callLater(loadVisibleRange)
    }

    property bool isLittle: false
    property bool onlyLove: false
    property int playlistId: -1

    signal play(int musicId, int listId)
    signal createMenu()

    function reset() {
        if (model !== null && typeof model.clear === "function") {
            model.clear()
            contentY = 0
            loadVisibleRange()
        }
    }

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: index
        isLittle: musicListView.isLittle
        playlistId: musicListView.playlistId
        visible: model.id > 0 && (!musicListView.onlyLove || model.isLove)
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
    }
}
