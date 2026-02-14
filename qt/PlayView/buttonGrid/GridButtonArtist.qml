import QtQuick
import PlayView

GridButtonBase {
    delegate: CoreButtonArtist {
        artistId: model.id
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }

    function listToModel(list) {
        list.forEach(id => {gridModel.append({id: id})})
    }
}
