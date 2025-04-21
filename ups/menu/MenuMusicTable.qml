import QtQuick
import Tiko
import Ups

MenuMusic {
    property int tableId: -1

    TikoMenu{
        title: qsTr("移动到")
        icon.source: "qrc:/image/move.png"
        enabled: tableId !== -1

        Repeater{
            delegate: moveMenu
            model: ListModel{
                id: moveMenuData
            }
        }
        Component {
            id: moveMenu
            TikoMenuItem {
                text: CoreData.table[aim]["name"]
            }
        }

        onOpened: {
            moveMenuData.clear()
            for(var i=0; i<CoreData.table.length; i++){
                moveMenuData.append({aim: i})
            }
        }
    }

    TikoMenuItem {
        text: qsTr("从播放列表中移除")
    }
}
