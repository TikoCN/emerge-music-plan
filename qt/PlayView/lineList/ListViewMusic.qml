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
    property bool isLittle: false
    property bool onlyLove: false
    signal play(int musicId, int listId)
    signal createMenu()

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
        onPlayMusic: (musicId, listId) => {musicListView.play(musicId, listId)}
        isLittle: musicListView.isLittle
        visible: {
            if (onlyLove && !isLove) {
                return false
            }
            else {
                return true
            }
        }
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentNormal
        radius: 10
    }

    function appendList(list) {
        for(var i=0; i<list.length; i++){
            musicModel.append({
                                  listId: i,
                                  id: list[i]
                              })
        }

        if (list.length !== CoreData.pageSize)
            dataLoader.loadIsFinish = true
        dataLoader.loadPos += list.length
    }

    function reset() {
        musicModel.clear()
        dataLoader.reset()
    }
}
