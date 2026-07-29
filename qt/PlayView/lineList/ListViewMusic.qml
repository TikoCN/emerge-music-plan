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
    ScrollBar.vertical: TikoBarV{}

    property bool isLittle: false
    property bool onlyLove: false

    signal play(int musicId, int listId)
    signal createMenu()

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: index
        isLittle: musicListView.isLittle
        visible: !onlyLove || model.isLove
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
    }
}