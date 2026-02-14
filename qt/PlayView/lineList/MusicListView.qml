import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView
import DataType

ListView{
    id: musicListView
    interactive: true
    spacing: 15
    clip: true
    reuseItems: true
    ScrollBar.vertical: TikoBarV{}

    property LoadBase dataLoader: LoadBase{}
    signal play()

    onAtYEndChanged: {
        if (atYEnd && orientation === ListView.Vertical) dataLoader.loadMore()
    }

    model: ListModel{
        id: musicModel
    }

    delegate: CoreMusicLine{
        width: musicListView.width
        listId: model.listId
        musicId: model.id
        onPlayMusic: musicListView.play()
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentNormal
        radius: 10
    }

    function listToModel(list) {
        for(var i=0; i<list.length; i++){
            musicModel.append({
                                  listId: i,
                                  id: list[i]
                              })
        }
    }

    function reset() {
        dataLoader.reset()
        musicModel.clear()
    }
}
