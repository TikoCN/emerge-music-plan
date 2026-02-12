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

    ScrollBar.vertical: TikoBarV{
    }


    model: ListModel{
        id: musicModel
    }

    delegate: CoreMusicLine{
        width: musicListView.width - 20
        listId: musicListId
        musicId: inMusicId
        onPlayMusic: MediaPlayer.buildPlayingListByMusicList(musicList, listId)
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentNormal
        radius: 10
    }


    function buildMusicList(musicList) {
        musicModel.clear()
        for(var i=0; i<musicList.length; i++){
            musicModel.append({
                                  musicListId: i,
                                  inMusicId: musicList[i]
                              })
        }
    }
}
