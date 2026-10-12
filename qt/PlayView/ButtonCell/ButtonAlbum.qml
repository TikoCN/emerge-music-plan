import QtQuick
import Tiko
import MediaerAPI
import PlayView

ButtonBase {
    id: albumButton

    name: model.name
    subtitle: BaseTool.typeConversion.timeToString(Number(model.duration))

    normalIcon: "qrc:/image/album.svg"
    loadIcon: "image://cover/albumFile?id=" +
              model.id.toString() +
              "&radius=10"
    onPage: CoreData.stackAlbum(model.id)
    onMenu: position => createMenu(this, position)
    onPlay: MediaPlayer.buildPlayingListByMusicList(musicList)

    Component {
        id: menuComponent
        MenuAlbum {
            albumId: model.id
            musicList: albumButton.musicList
            name: model.name
        }
    }

    function createMenu(parent, position){
        if (menuComponent.status === Component.Ready){
            let menu = menuComponent.createObject(parent)
            menu.popupAt(parent, position.x, position.y)
        }
        else
            console.log(menuComponent.errorString())
    }
}
