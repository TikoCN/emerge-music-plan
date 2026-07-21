import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI   
import Tiko
import PlayView
import DataType

PageBase {
    id: playerPlayList
    property int playlistId: -1
    property playListData playlist

    sort: playlist.sort
    duration: playlist.duration
    name: playlist.name
    musicCount: playlist.musicCount

    normalIcon: "qrc:/image/list.png"
    loadIcon: "image://cover/playlistFile?id=" +
              playlistId.toString() +
              "&radius=10"

    function setPlayListId(id){
        if (playlistId === id)
            return
        playlistId = id

        playlist = DataActive.getPlayListData(id);

        musicList.reset()
    }
    musicList.dataLoader.onLoadInitData: {
        let list = SQLData.getPlayListMusic(playlistId, CoreData.pageSize, 0, sort)
        musicList.appendList(list)
    }

    musicList.dataLoader.onLoadData: (index)=>{
                                         let list = SQLData.getPlayListMusic(playlistId, CoreData.pageSize, musicList.dataLoader.loadPos, sort)
                                         musicList.appendList(list)
                                     }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingPlayList(playlistId, listId)
                      }
}
