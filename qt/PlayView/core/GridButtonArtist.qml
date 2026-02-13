import QtQuick

GridButtonBase {
    delegate: CoreArtistButton {
        artistId: model.id
        width: realCellWidth
        onHeightChanged: setGridHeight(this)
    }
}
