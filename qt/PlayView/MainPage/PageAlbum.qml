import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageBase{
    id: albumPlayer
    property int albumId: -1

    sort: 0
    duration: 0
    name: ""
    musicCount: 0

    normalIcon: "qrc:/image/album.png"
    loadIcon: "image://cover/albumFile?id=" +
              albumId.toString() +
              "&radius=10"

    function setAlbumId(id){
        if(albumPlayer.albumId === id)
            return
        albumPlayer.albumId = id

        let album = AlbumLibrary.getAlbumData(id)
        name = album.name
        sort = album.sort
        duration = album.duration
        musicCount = album.musicCount

        musicList.reset()
    }

    musicList.dataLoader.onLoadInitData: {
        MusicLibrary.model().loadAlbumMusic(albumId, CoreData.pageSize, 0, sort)
    }

    musicList.dataLoader.onLoadData: (index)=>{
                                         MusicLibrary.model().loadMoreAlbumMusic(albumId, CoreData.pageSize, musicList.dataLoader.loadPos, sort)
                                     }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingAlbum(albumId, listId)
                      }
}