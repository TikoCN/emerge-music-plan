pragma Singleton
import QtQuick
import PlayView
import MediaerAPI
import Tiko
import DataType

QtObject {
    property var playlist: []

    property int pageSize: 100

    signal mainTurnMusicList(int page)
    signal mainTurnSeit()
    signal mainTurnMain()
    signal mainTurnLibraryPage()
    signal mainTurnAlbumPlayer(int album)
    signal mainTurnArtistPlayer(int artist)
    signal sendMsg(string msg, int type)
    signal clearData()
    signal sendErrorMsg(string msg)

    function editMusic(parent, musicId){
        var component = Qt.createComponent("toolEditMusicPage/ToolEditMusic.qml")

        if (component.status === Component.Ready) {
            var item = component.createObject(parent)
            item.build(musicId)
            item.show()
        }
        else {
            console.log(component.errorString())
        }
    }
}
