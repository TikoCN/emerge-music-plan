import QtQuick
import PlayView
import Tiko
import MediaerAPI

Item{
    id: style
    Canvas{
        id: canvas
        x: style.width * 0.5 * 0.1
        y: style.height * 0.1
        width: style.width * 0.8 * 0.5
        height: style.height * 0.8
        property var samples: []
        property int startAngle: 0
        onPaint: {
            var min = Math.min(canvas.width, canvas.height)
            var r = min * 0.4
            var ctx = getContext("2d")
            ctx.clearRect(0, 0, width, height)

            ctx.lineWidth = 3
            var ringColor = Qt.rgba(Setting.themeColor.r, Setting.themeColor.g, Setting.themeColor.b, 0.3)
            ctx.strokeStyle = ringColor

            ctx.beginPath()

            canvas.startAngle += 1
            var angleStep = 3// 每3度一个样本
            var totalSteps = 360 / angleStep
            var amplitude = 100

            // 确保样本数量足够，不足时补零
            for (var i = 0; i < totalSteps; i++) {
                var angle = i * angleStep + canvas.startAngle
                var radians = angle * Math.PI / 180

                // 获取样本值（假设samples是0~1的归一化数组）
                var sampleValue = (samples[i] || 0) * amplitude // 放大样本效果
                if(Math.abs(sampleValue) < amplitude * 0.1){
                    sampleValue = 0
                }

                // 极坐标转笛卡尔坐标
                var x = canvas.width/2 + Math.cos(radians) * (r + sampleValue)
                var y = canvas.height/2 + Math.sin(radians) * (r + sampleValue)

                if (i === 0) {
                    ctx.moveTo(x, y) // 路径起点
                } else {
                    ctx.lineTo(x, y)
                }
            }
            ctx.closePath() // 闭合路径

            // 确保样本数量足够，不足时补零
            for (i = 0; i < totalSteps; i++) {
                angle = i * angleStep + canvas.startAngle
                radians = angle * Math.PI / 180

                // 获取样本值（假设samples是0~1的归一化数组）
                sampleValue = (samples[i] || 0) * amplitude // 放大样本效果
                if(Math.abs(sampleValue) < amplitude * 0.05){
                    sampleValue = 0
                }

                // 极坐标转笛卡尔坐标
                x = canvas.width/2 + Math.cos(radians) * (r - sampleValue)
                y = canvas.height/2 + Math.sin(radians) * (r - sampleValue)

                if (i === 0) {
                    ctx.moveTo(x, y) // 路径起点
                } else {
                    ctx.lineTo(x, y)
                }
            }

            ctx.closePath() // 闭合路径

            // 确保样本数量足够，不足时补零
            for (i = 0; i < totalSteps; i++) {
                angle = i * angleStep + canvas.startAngle
                radians = angle * Math.PI / 180

                // 获取样本值（假设samples是0~1的归一化数组）
                sampleValue = (samples[i] || 0) * amplitude // 放大样本效果
                if(Math.abs(sampleValue) < amplitude * 0.2){
                    continue
                }

                // 极坐标转笛卡尔坐标
                x = canvas.width/2 + Math.cos(radians) * (r - sampleValue)
                y = canvas.height/2 + Math.sin(radians) * (r - sampleValue)
                ctx.moveTo(x, y) // 路径起点

                // 极坐标转笛卡尔坐标
                x = canvas.width/2 + Math.cos(radians) * (r + sampleValue)
                y = canvas.height/2 + Math.sin(radians) * (r + sampleValue)
                ctx.lineTo(x, y)
            }
            ctx.stroke()
        }

        Component.onCompleted: canvas.requestPaint()
        //关联
        Connections{
            target: MediaPlayer
            function onCppDrawLine(outSamples){
                canvas.samples = outSamples
                canvas.requestPaint()
            }
        }
    }

    TikoTextLine{
        id: title
        x: style.width * 0.5 + style.width * 0.5 * 0.1
        y: style.height * 0.2
        width: style.width * 0.5 * 0.8
        exSize: 8
    }

    TikoTextLine{
        id: artist
        x: title.x
        width: title.width
        anchors.top: title.bottom
        exSize: 8
    }

    Connections{
        target:MediaPlayer.player
        function onSourceChanged(){
            artist.text = MediaPlayer.playingMusic.artist
            title.text = MediaPlayer.playingMusic.title
        }
    }

    Component.onCompleted: {
        artist.text = MediaPlayer.playingMusic.artist
        title.text = MediaPlayer.playingMusic.title
    }
}
