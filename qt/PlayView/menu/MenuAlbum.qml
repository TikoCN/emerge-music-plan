import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoMenu {
    id: albumMenu
    property var inputPopup: null
    property int albumId: -1
    property var musicList: []
    property string name: ""

    onClosed: {
        if (inputPopup === null)
            destroy()
    }

    TikoMenuItem {
        text: qsTr("播放")
        onClicked: MediaPlayer.buildPlayingAlbum(albumId)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendPlayingAlbum(albumId)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertPlayingAlbum(albumId)
    }

    TikoMenuSpeacer{}

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.png"

        Repeater{
            delegate: addMenu
            model: ListModel{
                id: addMenuData
            }
        }
        Component {
            id: addMenu
            TikoMenuItem {
                text: CoreData.playlist[aim]["name"]
                onTriggered: AlbumLibrary.addMusicToPlaylist(albumMenu.name, CoreData.playlist[aim]["name"])
                enabled: !CoreData.playlist[aim]["isDir"]
            }
        }

        onOpened: {
            addMenuData.clear()
            for(let i=0; i<CoreData.playlist.length; i++){
                const data = CoreData.playlist[i];
                addMenuData.append({aim: i})
            }
        }
    }

    TikoMenuItem {
        text: qsTr("修改名称")
        onClicked: openInput()
    }

    TikoMenuSpeacer{}

    TikoMenuItem {
        text: qsTr("显示专辑")
        onClicked: CoreData.stackAlbum(albumId)
    }

    Component {
        id: inputComponent
        TikoPopupInput {
            orgText: name
            onAccept: AlbumLibrary.moveMusic(name, newName)
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            inputPopup = inputComponent.createObject(parent)
            inputPopup.open()
        }
    }
}
