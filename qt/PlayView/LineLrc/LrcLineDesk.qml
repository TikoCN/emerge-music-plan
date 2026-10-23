import QtQuick
import MediaerAPI
import Tiko
import PlayView

Item {
    id: deskLine

    property var startList: []
    property var endList: []
    property var textList: []
    property bool hasLyrics: textList.length > 0 && textList.join("") !== ""

    implicitHeight: lyricLine.implicitHeight

    LrcLineNormal {
        id: lyricLine
        width: deskLine.width
        lrcId: MediaPlayer.playingLrcId
        startList: deskLine.startList
        endList: deskLine.endList
        textList: deskLine.textList
        lrcFont: Setting.deskLrcFont
        playingColor: Setting.lrcPlayingColor
        normalColor: Setting.lrcNormalColor
    }

    Text {
        anchors.centerIn: parent
        visible: !deskLine.hasLyrics
        text: "Music  Lryic"
        color: TikoSeit.theme.colorTextDefault
        font: Setting.deskLrcFont
    }

    onLrcIdChanged: load()

    property int lrcId: MediaPlayer.playingLrcId

    Component.onCompleted: load()

    function load() {
        const json = MediaPlayer.getLrcJsonObject(lrcId)
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
            deskLine.lrcId = playingLrcId
        }
    }
}
