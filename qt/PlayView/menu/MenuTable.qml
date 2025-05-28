import QtQuick
import Tiko
import PlayView
import DataType
import MediaerAPI

TikoMenu{
    id: tableMenu
    onClosed: {
        destroy()
        if (inputPopup === null)
            destroy()
    }
    property var inputPopup: null
    property int tableId: -1
    property TableData table: Core.getTable(tableId)

    TikoMenuItem{
        text: qsTr("打开本地文件夹")
        enabled: table.isDir
        onClicked: Base.deskOpenFile(table.url)
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
                text: CoreData.table[aim]["name"]
                enabled: !CoreData.table[aim]["isDir"]
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<CoreData.table.length; i++){
                addMenuData.append({aim: i})
            }
        }
    }

    Component {
        id: inputComponent
        InputTableName {
            tableId: tableMenu.tableId
        }
    }

    function openInput(){
        if (inputComponent.status === Component.Ready) {
            inputPopup = inputComponent.createObject(tableMenu.parent)
            inputPopup.open()
        }
    }
}
