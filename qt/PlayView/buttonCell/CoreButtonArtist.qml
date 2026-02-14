import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

CoreButtonBase {
    id: artistButton

    property int artistId: -1
    property var musicList: []

    normalIcon: "qrc:/image/artist.png"
    loadIcon: "image://cover/artistFile?id=" +
              artistId.toString() +
              "&radius=10"
    onPage: CoreData.mainTurnArtistPlayer(artistId)
    onMenu: createMenu(this)
    onPlay: MediaPlayer.buildPlayingListByMusicList(musicList)

    onArtistIdChanged: {
        const json = DataActive.getArtistJson(artistId);
        name = json.artist
        subtitle = BaseTool.typeConversion.timeToString(Number(json.duration))
        musicList = json.musicList
    }

    Component {
        id: menuComponent
        MenuArtist {
            artistId: artistButton.artistId
            musicList: artistButton.musicList
            name: artistButton.name
        }
    }

    function createMenu(parent){
        if (menuComponent.status === Component.Ready){
            let menu = menuComponent.createObject(parent)
            menu.popup()
        }
        else
            console.log(menuComponent.errorString())
    }
}
