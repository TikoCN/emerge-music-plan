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
    property var playlist: []

    //大小尺寸
    property int fontH: 0
    property int fontW: 0
    property int cellItemHeight: cellImageHeight + TikoSeit.normalLineHeight * 2
    property int cellItemWidth: cellImageWidth + 2 * cellItemSpace
    property int cellItemSpace: 3
    property int cellImageHeight: 160
    property int cellImageWidth: 160
    property int recomLineHeight: cellItemHeight + 20
    property int cellTextHeight: 0

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
