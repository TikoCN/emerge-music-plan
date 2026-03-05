import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

CoreButtonBase {
    id: albumButton

    property int albumId: -1
    property albumData album
    name: album.name
    subtitle: subtitle = BaseTool.typeConversion.timeToString(Number(album.duration))

    normalIcon: "qrc:/image/album.png"
    loadIcon: "image://cover/albumFile?id=" +
              albumId.toString() +
              "&radius=10"
    onPage: CoreData.mainTurnAlbumPlayer(albumId)
    onMenu: createMenu(this)
    onPlay: MediaPlayer.buildPlayingListByMusicList(musicList)

    onAlbumIdChanged: album = DataActive.getAlbumData(albumId)

    Component {
        id: menuComponent
        MenuAlbum {
            albumId: albumButton.albumId
            musicList: albumButton.musicList
            name: albumButton.name
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
