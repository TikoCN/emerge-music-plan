import QtQuick
import Tiko
import MediaerAPI
import PlayView
import DataType

ShowPageBase{
    id: albumPlayer
    property int albumId: -1

    normalIcon: "qrc:/image/album.png"
    loadIcon: "image://cover/albumFile?id=" +
              albumId.toString() +
              "&radius=10"

    function setAlbumId(albumId){
        if(albumPlayer.albumId === albumId)
            return
        albumPlayer.albumId = albumId

        const json = DataActive.getAlbumJson(albumId);
        sort = json.sort
        duration = json.duration
        name = json.name
        musicCount = json.musicCount

        musicList.reset()
    }

    musicList.dataLoader.onLoadInitData: {
        let list = SQLData.getAlbumMusic(albumId, CoreData.pageSize, 0, sort)
        musicList.appendList(list)
    }

    musicList.dataLoader.onLoadData: (index)=>{
                                         let list = SQLData.getAlbumMusic(albumId, CoreData.pageSize, musicList.dataLoader.loadPos, sort)
                                         musicList.appendList(list)
                                     }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingAlbum(albumId, listId)
                      }
}
