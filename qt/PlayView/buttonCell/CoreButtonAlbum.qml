import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

CoreButtonBase {
    id: albumButton

    property int albumId: -1
    property var musicList: []

    normalIcon: "qrc:/image/album.png"
    loadIcon: "image://cover/albumFile?id=" +
              albumId.toString() +
              "&radius=10"
    onPage: CoreData.mainTurnAlbumPlayer(albumId)
    onMenu: createMenu(this)
    onPlay: MediaPlayer.buildPlayingListByMusicList(musicList)

    onAlbumIdChanged: {
        const json = DataActive.getAlbumJson(albumId);
        name = json.album
        subtitle = BaseTool.typeConversion.timeToString(Number(json.duration))
        musicList = json.musicList
    }

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
