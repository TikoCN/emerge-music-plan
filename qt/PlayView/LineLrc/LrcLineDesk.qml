import QtQuick
import MediaerAPI
import Tiko
import PlayView

LrcLineBase {
    playingColor: Setting.lrcPlayingColor
    normalColor: Setting.lrcNormalColor
    lrcFont: Setting.deskLrcFont
    isPlay: true
    aloneLine: true
    lrcId: MediaPlayer.playingLrcId

    onLrcIdChanged: load()

    function load () {
        var json = MediaPlayer.getLrcJsonObject(lrcId)
        startList = BaseTool.typeConversion.stringToLongList(json.startList)
        endList = BaseTool.typeConversion.stringToLongList(json.endList)
        textList = BaseTool.typeConversion.stringToStringList(json.textList)
        if (textList.length === 0 || textList.join("") === "") {
            textList = ["♪♪♪"]
        }
    }

    Connections {
        target: MediaPlayer
        function onPlayingLrcIdChanged(playingLrcId) {
            lrcId = playingLrcId
        }
    }
}
