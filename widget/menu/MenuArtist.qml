import QtQuick
import Tiko
import DataCore
import MediaerAPI
import Widget

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
                text: Core.tableList[aim].name
                onTriggered: Core.tableInsertMusic(aim, artist)
                enabled: !Core.tableList[aim].isDir
            }
        }

        onOpened: {
            addMenuData.clear()
            for(var i=0; i<Core.tableList.length; i++){
                addMenuData.append({aim: i})
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
