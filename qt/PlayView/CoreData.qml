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
    property int cellItemHeight: cellImageHeight + 2 * fontH
    property int cellItemWidth: cellImageWidth + 14
    property int cellItemSpace: 7
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

    function setCellSize(parent) {

        //临时控件
        const metrics = Qt.createQmlObject(`
                                           import QtQuick 2.15
                                           TextMetrics {
                                           font.pixelSize: ${Setting.mainFont.pixelSize}
                                           font.family: "${Setting.mainFont.family}"
                                           // 包含上下延伸字符，确保最大高度
                                           text: "Ágḿ･高"
                                           }
                                           `, parent);

        //获取纯文本高度（边界矩形高度）
        cellTextHeight = metrics.boundingRect.height;

        cellItemHeight = cellImageHeight + cellTextHeight * 2 + cellItemSpace * 3
        cellImageWidth = cellImageWidth + cellItemSpace * 2
        recomLineHeight = cellItemHeight + cellItemSpace * 2

        metrics.destroy();
    }
}
