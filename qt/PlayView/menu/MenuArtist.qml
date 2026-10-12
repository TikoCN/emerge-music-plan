import QtQuick
import QtQuick.Window
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoMenu {
    id: artistMenu
    property var inputPopup: null
    property int artistId: -1
    property string name: ""

    onClosed: {
        if (inputPopup === null)
            destroy()
    }

    TikoMenuItem {
        text: qsTr("播放")
        onClicked: MediaPlayer.buildPlayingArtist(artistId)
        icon.source: "qrc:/image/play.svg"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendPlayingArtist(artistId)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.insertPlayingArtist(artistId)
    }

    TikoMenuSpacer{}

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.svg"

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
                onTriggered: ArtistLibrary.addMusicToPlaylist(artistMenu.name, CoreData.playlist[aim]["name"])
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
        text: qsTr("编辑信息")
        onClicked: openInput()
    }

    TikoMenuSpacer{}

    TikoMenuItem {
        text: qsTr("显示作曲家")
        onClicked: CoreData.stackArtist(artist)
    }

    Component {
        id: inputComponent
        TikoInputWindow {
            orgText: name
            onAccept: ArtistLibrary.moveMusic(name, inputText)
            onClosing: {
                const currentInput = inputPopup
                inputPopup = null
                Qt.callLater(function() {
                    currentInput.destroy()
                    artistMenu.destroy()
                })
            }
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            const ownerWindow = artistMenu.parent.Window.window
            inputPopup = inputComponent.createObject(null, {
                x: ownerWindow.x + (ownerWindow.width - 600) / 2,
                y: ownerWindow.y + (ownerWindow.height - 400) / 2
            })
            if (inputPopup !== null)
                inputPopup.open()
        }
    }
}
