import QtQuick

GridButtonBase {
    delegate: CoreAlbumButton {
        albumId: model.id
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }
}
