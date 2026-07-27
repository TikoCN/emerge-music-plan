import QtQuick
import Tiko
import MediaerAPI
import PlayView

ButtonBase {
    id: artistButton

    name: model.name
    subtitle: BaseTool.typeConversion.timeToString(Number(model.duration))

    normalIcon: "qrc:/image/artist.png"
    loadIcon: "image://cover/artistFile?id=" +
              model.id.toString() +
              "&radius=10"
    onPage: CoreData.stackArtist(model.id)
    onMenu: createMenu(this)
    onPlay: MediaPlayer.buildPlayingArtist(model.id)

    Component {
        id: menuComponent
        MenuArtist {
            artistId: model.id
            name: model.name
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