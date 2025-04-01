import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import MediaerAPI
import Tiko

Canvas {
    id: drawLoadLine
    height: 0

    property color playingColor: "#ffffffff"
    property color normalColor: "#40ffffff"
    property var core
    property int lrcId: core.id
    property bool isDraw: false
    property double r: lrcFont.pixelSize / 2
    property font lrcFont: Setting.mainLrcFont

    onPaint: {
        var ctx = drawLoadLine.getContext("2d")
        var width = drawLoadLine.width
        var height = drawLoadLine.height
        // 清除画布
        var r = drawLoadLine.r
        var centerY = height / 2
        var space = 10
        var boreder = 20
        ctx.clearRect(0, 0, width, height);

        // 渐变色
        var overF = (MediaPlayer.player.position - core.startTime)/(core.endTime - core.startTime)
        overF = overF || 0
        overF = overF > 1 || 1
        var gradient = ctx.createLinearGradient(boreder, centerY - r, boreder + (space + 2*r)*3 + 2*r, centerY - r);

        // 添加颜色停止点
        gradient.addColorStop(0, drawLoadLine.playingColor);
        gradient.addColorStop(overF, drawLoadLine.playingColor);
        gradient.addColorStop(overF, drawLoadLine.normalColor);
        gradient.addColorStop(1, drawLoadLine.normalColor)

        ctx.beginPath()
        for(var i=0; i<3; i++){
            var startX = boreder + (space + 2 * r) * i
            ctx.ellipse(startX, centerY - r, 2 * r, 2 * r)
        }
        ctx.fillStyle = gradient
        ctx.fill()
    }

    // 呼吸动画
    SequentialAnimation {
        running: isDraw
        loops: Animation.Infinite

        NumberAnimation {
            target: drawLoadLine
            properties: "r"
            from: lrcFont.pixelSize / 2
            to: lrcFont.pixelSize / 2 + 2
            duration: 1000
            easing.type: Easing.InQuint
        }

        NumberAnimation {
            target: drawLoadLine
            properties: "r"
            from: lrcFont.pixelSize / 2 + 2
            to: lrcFont.pixelSize / 2
            duration: 1000
            easing.type: Easing.OutQuint
        }
    }

    // 展示动画
    NumberAnimation on height {
        id: showAnimation
        from: 0
        to: lrcFont.pixelSize + 30
        duration: 300
        easing.type: Easing.InQuint
    }

    // 关闭动画
    NumberAnimation on height {
        id: closeAnimation
        from: lrcFont.pixelSize + 30
        to: 0
        duration: 300
        easing.type: Easing.InQuint
    }

    Connections{
        target: core
        function onUpdate(){
            drawLoadLine.requestPaint()
        }
    }

    Connections{
        target: MediaPlayer

        function onPlayingLrcIdChange(){
            if(isDraw != (MediaPlayer.playingLrc.id === lrcId)) {
                isDraw = (MediaPlayer.playingLrc.id === lrcId)

                if (isDraw) {
                    showAnimation.start()
                }
                else {
                    closeAnimation.start()
                }
            }
        }
    }
}

