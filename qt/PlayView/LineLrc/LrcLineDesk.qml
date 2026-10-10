import QtQuick
import MediaerAPI
import Tiko
import PlayView

LrcLineBase {
    id: deskLine
    playingColor: Setting.lrcPlayingColor
    normalColor: Setting.lrcNormalColor
    lrcFont: Setting.deskLrcFont
    isPlay: true
    aloneLine: true
    lrcId: MediaPlayer.playingLrcId
    property bool hasLyrics: textList.length > 0 && textList.join("") !== ""

    onLrcIdChanged: load()

    Text {
        anchors.centerIn: parent
        visible: !deskLine.hasLyrics
        text: "Music  Lryic"
        color: TikoSeit.theme.colorTextDefault
        font: Setting.deskLrcFont
    }

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
