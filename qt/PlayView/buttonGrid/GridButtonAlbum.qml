import QtQuick
import PlayView

GridButtonBase {
    delegate: CoreButtonAlbum {
        albumId: model.id
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }

    function listToModel(list) {
        list.forEach(id => {gridModel.append({id: id})})
    }
}
