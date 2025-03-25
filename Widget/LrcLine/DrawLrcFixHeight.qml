import QtQuick
import QtQuick.Controls.Basic
import TikoAPI
import Tiko
Canvas {
    id: drawLrcFixHeight
    height: 0

    property color playingColor: "#ffffffff"
    property color normalColor: "#40ffffff"
    property var playLrc: MediaPlayer.playingLrc

    onPaint: {
        var ctx = getContext("2d")
        var width = drawLrcFixHeight.width
        var height = drawLrcFixHeight.height
        // 清除画布
        var centerY = height / 2
        var space = 10
        var boreder = 20
        var endList = playLrc.endList
        var startList = playLrc.startList
        var textList = playLrc.textList
        var startY = (height - TikoSeit.fontPixelSize * 3) / 2
        var startX = 20
        ctx.clearRect(0, 0, width, height);
        ctx.font = "bold "+ oneFontMetrics.font.pixelSize.toString() +"px 'Microsoft YaHei', SimSun, sans-serif"

        for (var i=0; i<textList; i++) {
            oneFontMetrics.text = textList[i]
            // 渐变色
            var overF = (MediaPlayer.player.position - startList[i]) / (endList[i] - startList[i])
            overF = overF || 0
            overF = overF > 1 || 1
            var gradient = ctx.createLinearGradient(startX, startX + oneFontMetrics.advanceWidth + 3);

            // 添加颜色停止点
            gradient.addColorStop(0, drawLrcFixHeight.playingColor);
            gradient.addColorStop(overF, drawLrcFixHeight.playingColor);
            gradient.addColorStop(overF, drawLrcFixHeight.normalColor);
            gradient.addColorStop(1, drawLrcFixHeight.normalColor)

            ctx.fillStyle = gradient
            ctx.fillText(textList[i], startX, startY - overF * 3)
            startX += oneFontMetrics.advanceWidth + 3
        }
    }

    TextMetrics{
        id: oneFontMetrics
        font: Setting.mainLrcFont
    }

    Connections{
        target: playLrc
        function onUpdate(){
            drawLrcFixHeight.requestPaint()
        }
    }

    Connections{
        target: MediaPlayer

        function onPlayingLrcIdChange(){
            playLrc = MediaPlayer.playingLrc
        }
    }
}
