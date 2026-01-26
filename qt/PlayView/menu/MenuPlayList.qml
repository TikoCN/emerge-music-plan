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
            model: ListModel{
                id: addMenuData
            }
        }
        Component {
            id: addMenu
            TikoMenuItem {
                text: CoreData.playlist[aim]["name"]
                enabled: !CoreData.playlist[aim]["isDir"]
            }
        }

        onOpened: {
            addMenuData.clear()
            for(let i=0; i<CoreData.playlist.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }

    Component {
        id: inputComponent
        InputPlayListName {
            playlistId: playlistMenu.playlistId
            orgText: name
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            inputPopup = inputComponent.createObject(playlistMenu.parent)
            inputPopup.open()
        }
    }
}
