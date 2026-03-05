import QtQuick
import Tiko
import MediaerAPI
import DataType
import PlayView

CoreButtonBase {
    id: musicButton

    property int musicId: -1
    property int type: 0
    property musicData music
    signal playMusic()

    name: music.title
    subtitle: music.artist
    loadIcon: "image://cover/musicFile?id=" +
              musicId.toString() +
              "&radius=10"
    normalIcon: "qrc:/image/music.png"

    onMenu: createMenu(this)
    onPage: playMusic()
    onPlay: playMusic()

    onMusicIdChanged: music = DataActive.getMusicData(musicId)

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
