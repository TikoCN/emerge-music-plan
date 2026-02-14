import QtQuick
import Tiko
import MediaerAPI
import DataType
import PlayView

CoreButtonBase {
    id: musicButton

    property int musicId: -1
    property int type: 0
    signal playMusic()

    loadIcon: "image://cover/musicFile?id=" +
              musicId.toString() +
              "&radius=10"
    normalIcon: "qrc:/image/exe.png"

    onMenu: createMenu(this)
    onPage: playMusic()
    onPlay: playMusic()

    onMusicIdChanged: {
        const Json = DataActive.getMusicJson(musicId);
        name = Json.title
        subtitle = Json.artist
    }

    Component {
        id: menuComponent
        MenuMusic {
            musicId: musicButton.musicId
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
