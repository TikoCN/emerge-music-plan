import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoMenu {
    id: albumMenu
    onClosed: {
        Core.releaseAlbum(albumId)
        if (inputPopup === null)
            destroy()
    }
    property var inputPopup: null
    property AlbumData album: Core.getAlbum(albumId)
    property int albumId: -1

    TikoMenuItem {
        text: qsTr("播放")
        onClicked: MediaPlayer.playMusic(album)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendMusic(album)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertMusic(album)
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
                text: name
                onTriggered: Core.tableAppendMusic(tableId, album.musicList)
                enabled: !isDir
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<CoreData.table.length; i++){
                var data = CoreData.table[i]
                addMenuData.append({
                                       name: data["name"],
                                       isDir: data["isDir"],
                                       table: data["tableId"]
                                   })
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
        onClicked: CoreData.mainTurnAlbumPlayer(album)
    }

    Component {
        id: inputComponent
        InputAlbumName {
            albumId: albumMenu.albumId
            onClosed: {
                destroy()
                albumMenu.destroy()
            }
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            inputPopup = inputComponent.createObject(parent)
            inputPopup.open()
        }
    }
}
