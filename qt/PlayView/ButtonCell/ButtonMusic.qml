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
    normalIcon: "qrc:/image/music.svg"

    onMenu: position => createMenu(this, position)
    onPage: playMusic()
    onPlay: playMusic()

    Component {
        id: menuComponent
        MenuMusic {
            musicId: model.id
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
