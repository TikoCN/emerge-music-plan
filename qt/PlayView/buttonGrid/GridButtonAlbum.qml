import QtQuick
import PlayView
import MediaerAPI

GridButtonBase {
    delegate: CoreButtonAlbum {
        albumId: model.id
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }

    function appendList(list) {
        list.forEach(id => {gridModel.append({id: id})})
        if (list.length !== CoreData.pageSize)
            dataLoader.loadIsFinish = true
        dataLoader.loadPos += list.length
    }
}
