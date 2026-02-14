import QtQuick
import PlayView

GridButtonBase {
    delegate: CoreButtonMusic {
        musicId: model.id
        width: realCellWidth
        onPlayMusic: MediaPlayer.buildPlayingListByMusicId(musicId)
        onHeightChanged: setGridHeight(this)
    }

    function listToModel(list) {
        list.forEach(id => {gridModel.append({id: id})})
    }
}
