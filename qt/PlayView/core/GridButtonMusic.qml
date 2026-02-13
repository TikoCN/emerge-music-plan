import QtQuick

GridButtonBase {
    delegate: CoreMusicButton {
        musicId: model.id
        width: realCellWidth
        onPlayMusic: MediaPlayer.buildPlayingListByMusicId(musicId)
        onHeightChanged: setGridHeight(this)
    }
}
