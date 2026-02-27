import QtQuick
import PlayView
import MediaerAPI

GridButtonBase {
    delegate: CoreButtonMusic {
        musicId: model.id
        width: realCellWidth
        onPlayMusic: MediaPlayer.buildPlayingList([musicId])

        onHeightChanged: setGridHeight(this)
    }

    function appendList(list) {
        list.forEach(id => {gridModel.append({id: id})})
        if (list.length !== CoreData.pageSize)
            dataLoader.loadIsFinish = true
        dataLoader.loadPos += list.length
    }
}
