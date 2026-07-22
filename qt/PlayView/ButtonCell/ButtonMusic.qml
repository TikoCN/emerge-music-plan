import QtQuick
import Tiko
import MediaerAPI
import PlayView

ButtonBase {
    id: musicButton

    property int type: 0
    signal playMusic()

    name: model.title
    subtitle: model.artist
    loadIcon: "image://cover/musicFile?id=" +
              model.id.toString() +
              "&radius=10"
    normalIcon: "qrc:/image/music.png"

    onMenu: createMenu(this)
    onPage: playMusic()
    onPlay: playMusic()

    Component {
        id: menuComponent
        MenuMusic {
            musicId: model.id
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