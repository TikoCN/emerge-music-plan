import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageBase{
    id: artistPlayer
    property int artistId: -1

    sort: 0
    duration: 0
    name: ""
    musicCount: 0

    normalIcon: "qrc:/image/artist.png"
    loadIcon: "image://cover/artistFile?id=" +
              artistId.toString() +
              "&radius=10"

    function setArtistId(id){
        if(artistPlayer.artistId === id)
            return
        artistPlayer.artistId = id

        let artist = ArtistLibrary.getArtistData(id)
        name = artist.name
        sort = artist.sort
        duration = artist.duration
        musicCount = artist.musicCount

        musicList.reset()
    }

    musicList.dataLoader.onLoadInitData: {
        MusicLibrary.model().loadArtistMusic(artistId, CoreData.pageSize, 0, sort)
    }

    musicList.dataLoader.onLoadData: (index)=>{
                                         MusicLibrary.model().loadMoreArtistMusic(artistId, CoreData.pageSize, musicList.dataLoader.loadPos, sort)
                                     }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingArtist(artistId, listId)
                      }
}