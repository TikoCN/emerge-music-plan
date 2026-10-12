import QtQuick
import QtQuick.Window
import Tiko
import PlayView
import DataType
import MediaerAPI

TikoMenu{
    id: playlistMenu
    onClosed: {
        if (inputPopup === null)
            destroy()
    }
    property var inputPopup: null
    property int playlistId: -1
    property bool isDir: false
    property string name: ""

    TikoMenuItem{
        text: qsTr("打开本地文件夹")
        enabled: isDir
        onClicked: BaseTool.fileManagement.openPlaylistDir(playlistMenu.playlistId)
    }

    TikoMenuItem{
        text: qsTr("重命名")
        onClicked: openInput()
    }

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.svg"

        Repeater{
            delegate: addMenu
            model: PlaylistModel {
                type: PlaylistModel.UserModel
            }
        }
        Component {
            id: addMenu
            TikoMenuItem {
                text: model.name
                onTriggered: PlaylistLibrary.addMusicToPlaylist(playlistMenu.name, model.name)
            }
        }
    }

    Component {
        id: inputComponent
        TikoInputWindow {
            orgText: name
            onAccept: PlaylistLibrary.moveMusic(name, inputText)
            onClosing: {
                const currentInput = inputPopup
                inputPopup = null
                Qt.callLater(function() {
                    currentInput.destroy()
                    playlistMenu.destroy()
                })
            }
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            const ownerWindow = playlistMenu.parent.Window.window
            inputPopup = inputComponent.createObject(null, {
                x: ownerWindow.x + (ownerWindow.width - 600) / 2,
                y: ownerWindow.y + (ownerWindow.height - 400) / 2
            })
            if (inputPopup !== null)
                inputPopup.open()
        }
    }
}
