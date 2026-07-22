pragma Singleton
import QtQuick
import PlayView
import MediaerAPI
import Tiko
import DataType

QtObject {
    property var playlist: []

    property int pageSize: 30
    property TikoFrameless windows

    signal mainTurnMusicList(int page)
    signal mainTurnSeit()
    signal mainTurnMain()
    signal mainTurnLibraryPage()
    signal mainTurnAlbumPlayer(int album)
    signal mainTurnArtistPlayer(int artist)
    signal sendMsg(string msg, int type)
    signal clearData()
    signal sendErrorMsg(string msg)

    signal windowShowMin()
    signal windowShowMax()
    signal windowClose()

    signal autoUpdateUI()

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

    property Timer internalTimer: Timer {
        interval: 20
        running: true
        repeat: true
        onTriggered: autoUpdateUI()
    }
}
