import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

ButtonBase {
    id: artistButton

    property int artistId: -1
    property artistData artist
    name: artist.name
    subtitle: BaseTool.typeConversion.timeToString(Number(artist.duration))


    normalIcon: "qrc:/image/artist.png"
    loadIcon: "image://cover/artistFile?id=" +
              artistId.toString() +
              "&radius=10"
    onPage: CoreData.mainTurnArtistPlayer(artistId)
    onMenu: createMenu(this)
    onPlay: MediaPlayer.buildPlayingArtist(artistId)

    onArtistIdChanged: artist = DataActive.getArtistData(artistId)

    Component {
        id: menuComponent
        MenuArtist {
            artistId: artistButton.artistId
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
