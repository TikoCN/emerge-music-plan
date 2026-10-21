import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView

Item {
    id: lyricLine

    property color playingColor: TikoSeit.theme.colorFgDefault
    property color normalColor: TikoSeit.theme.colorFgHint
    property font lrcFont: Setting.lrcFont
    property int lrcId: -1
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []
    property bool isPlay: MediaPlayer.playingLrcId === lrcId
    property bool hovered: false
    property bool aloneLine: false

    implicitHeight: lyricSceneGraph.implicitHeight + TikoSeit.emphasizeMargins * 2
    height: implicitHeight

    LrcSceneGraphItem {
        id: lyricSceneGraph
        x: TikoSeit.emphasizeMargins
        y: TikoSeit.emphasizeMargins
        width: Math.max(0, lyricLine.width - TikoSeit.emphasizeMargins * 2)
        height: implicitHeight
        visible: lyricLine.isPlay || lyricLine.hovered
        text: {
            let lyric = Array.from(lyricLine.textList).join("")
            for (let i = 0; i < lyricLine.helpTextList.length; i++) {
                lyric += "\n" + lyricLine.helpTextList[i]
            }
            return lyric
        }
        font: lyricLine.lrcFont
        color: lyricLine.playingColor
        normalColor: lyricLine.normalColor
        words: Array.from(lyricLine.textList)
        wordStarts: Array.from(lyricLine.startList)
        wordEnds: Array.from(lyricLine.endList)
        position: MediaPlayer.playingPosition
        playing: lyricLine.isPlay
    }

    MultiEffect {
        anchors.fill: lyricSceneGraph
        source: lyricSceneGraph
        visible: !lyricLine.isPlay && !lyricLine.hovered
        autoPaddingEnabled: false
        blurEnabled: true
        blurMax: 30
        blur: 0.5
    }
}
