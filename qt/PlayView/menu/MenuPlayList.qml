import QtQuick
import Tiko
import PlayView
import DataType
import MediaerAPI

TikoMenu{
    id: playlistMenu
    onClosed: {
        destroy()
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
        onClicked: BaseTool.fileManagement.deskOpenFile(playlist.url)
    }

    TikoMenuItem{
        text: qsTr("重命名")
        onClicked: openInput()
    }

    TikoMenu{
        title: qsTr("添加到...")
        icon.source: "qrc:/image/move.png"

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
        TikoPopupInput {
            orgText: name
            onAccept: PlaylistLibrary.moveMusic(name, newName)
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            inputPopup = inputComponent.createObject(playlistMenu.parent)
            inputPopup.open()
        }
    }
}
