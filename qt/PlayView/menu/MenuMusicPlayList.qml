import QtQuick
import Tiko
import PlayView

MenuMusic {
    property int playlistId: -1

    TikoMenu{
        title: qsTr("移动到")
        icon.source: "qrc:/image/move.png"
        enabled: playlistId !== -1

        Repeater{
            delegate: moveMenu
            model: ListModel{
                id: moveMenuData
            }
        }
        Component {
            id: moveMenu
            TikoMenuItem {
                text: CoreData.playlist[aim]["name"]
            }
        }

        onOpened: {
            moveMenuData.clear()
            for(let i=0; i<CoreData.playlist.length; i++){
                moveMenuData.append({aim: i})
            }
        }
    }

    TikoMenuItem {
        text: qsTr("从播放列表中移除")
    }
}
