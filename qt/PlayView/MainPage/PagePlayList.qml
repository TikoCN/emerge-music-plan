import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI   
import Tiko
import PlayView

PageBase {
    id: playerPlaylist
    property int playlistId: -1

    sort: 0
    duration: 0
    name: ""
    musicCount: 0

    normalIcon: "qrc:/image/list.png"
    loadIcon: "image://cover/playlistFile?id=" +
              playlistId.toString() +
              "&radius=10"

    function setPlaylistId(id){
        if (playlistId === id)
            return
        playlistId = id

        let playlist = PlaylistLibrary.getData(id);
        name = playlist.name
        sort = playlist.sort
        duration = playlist.duration
        musicCount = playlist.musicCount
        musicModel.playlistId = playlistId
        musicModel.clear()
    }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingPlaylist(playlistId, listId)
                      }

    musicList.model: MusicModel {
        id: musicModel
        type: MusicModel.PlaylistModel
    }
}