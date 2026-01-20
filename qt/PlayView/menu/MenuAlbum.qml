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
        onClicked: MediaPlayer.buildPlayingListByMusicList(musicList)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendPlayingListByMusicList(musicList)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertPlayingListByMusicList(musicList)
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
                onTriggered: DataActive.playlistAppendMusic(playlistId, albumId)
                enabled: !isDir
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<CoreData.playlist.length; i++){
                var data = CoreData.playlist[i]
                addMenuData.append({
                                       name: data["name"],
                                       isDir: data["isDir"],
                                       playlist: data["playlistId"]
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
        onClicked: CoreData.mainTurnAlbumPlayer(albumId)
    }

    Component {
        id: inputComponent
        InputAlbumName {
            albumId: albumMenu.albumId
            musicList: albumMenu.musicList
            orgText: albumMenu.name

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
