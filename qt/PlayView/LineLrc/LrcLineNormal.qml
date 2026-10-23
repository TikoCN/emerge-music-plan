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
    property real blurAmount: lyricLine.isPlay || lyricLine.hovered
                              ? 0
                              : Math.min(0.75, Math.abs(lyricLine.lrcId - MediaPlayer.playingLrcId) * 0.2)

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
        position: lyricLine.isPlay ? MediaPlayer.playingPosition : 0
        playing: lyricLine.isPlay
    }

    MultiEffect {
        anchors.fill: lyricSceneGraph
        source: lyricSceneGraph
        visible: lyricLine.blurAmount > 0
        // 为模糊扩散预留透明边距，避免在歌词行边界形成矩形截断。
        autoPaddingEnabled: true
        blurEnabled: true
        blurMax: 30
        blur: lyricLine.blurAmount
    }
}
