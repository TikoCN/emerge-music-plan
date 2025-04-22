import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

TikoMenu {
    id: artistMenu
    onClosed: destroy()
    property ArtistData artist

    TikoMenuItem {
        text: qsTr("播放")
        onClicked: MediaPlayer.playMusic(artist)
        icon.source: "qrc:/image/play.png"
    }

    TikoMenuItem {
        text: qsTr("添加到播放队列")
        onClicked: MediaPlayer.appendMusic(artist)
    }

    TikoMenuItem {
        text: qsTr("添加到正在下一首播放")
        onClicked: MediaPlayer.inserttMusic(artist)
    }

    TikoMenuSpeacer{}

    TikoMenu{
        title: qsTr("添加到")
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
                onTriggered: Core.tableAppendMusic(tableId, artist.musicList)
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
        text: qsTr("编辑信息")
    }

    TikoMenuSpeacer{}

    TikoMenuItem {
        text: qsTr("显示作曲家")
        onClicked: CoreData.mainTurnArtistPlayer(artist)
    }
}
