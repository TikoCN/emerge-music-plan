import QtQuick
import Tiko
import MediaerAPI
import PlayView
import DataType

ShowPageBase{
    id: artistPlayer
    property int artistId: -1
    property artistData artist
    sort: artist.sort
    duration: artist.duration
    name: artist.name
    musicCount: artist.musicCount

    normalIcon: "qrc:/image/artist.png"
    loadIcon: "image://cover/artistFile?id=" +
              artistId.toString() +
              "&radius=10"

    function setArtistId(artistId){
        if(artistPlayer.artistId === artistId)
            return
        artistPlayer.artistId = artistId

        artist = DataActive.getArtistData(artistId);

        musicList.reset()
    }

    musicList.dataLoader.onLoadInitData: {
        let list = SQLData.getArtistMusic(artistId, CoreData.pageSize, 0, sort)
        musicList.appendList(list)
    }

    musicList.dataLoader.onLoadData: (index)=>{
                                         let list = SQLData.getArtistMusic(artistId, CoreData.pageSize, musicList.dataLoader.loadPos, sort)
                                         musicList.appendList(list)
                                     }

    musicList.onPlay: (musicId, listId) => {
                          MediaPlayer.buildPlayingArtist(artistId, listId)
                      }
}
