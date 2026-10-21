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

    property bool isLittle: false
    property bool onlyLove: false
    property int playlistId: -1

    signal play(int musicId, int listId)
    signal createMenu()

    function reset() {
        if (model !== null && typeof model.clear === "function")
            model.clear()
    }

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: index
        isLittle: musicListView.isLittle
        playlistId: musicListView.playlistId
        visible: !musicListView.onlyLove || model.isLove
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
    }
}
