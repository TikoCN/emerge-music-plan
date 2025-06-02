pragma Singleton
import QtQuick
import PlayView
import MediaerAPI
import Tiko
import DataType

QtObject {
    property double timeWidth: 0.0
    property double editTimeWidth: 0.0
    property double playNumberWidth: 0.0
    property bool sizeChange: false
    property var table: []
    signal mainTurnMusicList(int page)
    signal mainTurnSeit()
    signal mainTurnMain()
    signal mainTurnAlbumPage()
    signal mainTurnAlbumPlayer(int album)
    signal mainTurnArtistPage()
    signal mainTurnArtistPlayer(int artist)
    signal sendMsg(string msg, int type)
    signal clearData()
    signal sendErrorMsg(string msg)

    function editMusic(parent, core){
        var component = Qt.createComponent("toolEditMusicPage/ToolEditMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent)
            item.build(core, core.getLrcData())
            item.show()
        }
        else {
            console.log(component.errorString())
        }
    }
}
