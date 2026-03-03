import QtQuick
import PlayView
import MediaerAPI
import Tiko

Canvas{
    id: canvas
    property color showColor: TikoSeit.theme.baseTheme.foregroundNormal
    property int startAngle: 0
    property int r: Math.min(canvas.width, canvas.height) * 0.3
    onPaint: {
        const ctx = getContext("2d");
        ctx.clearRect(0, 0, width, height)

        ctx.lineWidth = 3
        const ringColor = showColor
        ctx.strokeStyle = ringColor

        ctx.beginPath()

        //startAngle = Math.floor(Math.random() * 361)
        const samples = MediaPlayer.allSamples
        const angleStep = 3  // 每?度一个样本
        const totalSteps = 360.0 / angleStep
        const amplitude = r
        let angle = 0
        let radians = 0
        let i = 0
        let sampleValue = 0
        let x = 0, y = 0

        // 确保样本数量足够，不足时补零
        // 外圈
        for (i = 0; i < totalSteps; i++) {
            angle = i * angleStep + canvas.startAngle;
            radians = angle * Math.PI / 180;

            // 获取样本值 samples是0~1的归一化数组
            sampleValue = (samples[i] || 0) * amplitude

            // 极坐标转笛卡尔坐标
            x = canvas.width / 2 + Math.cos(radians) * (r + sampleValue);
            y = canvas.height / 2 + Math.sin(radians) * (r + sampleValue);

            if (i === 0) {
                ctx.moveTo(x, y) // 路径起点
            } else {
                ctx.lineTo(x, y)
            }
        }
        ctx.closePath() // 闭合路径

        // 确保样本数量足够，不足时补零
        // 内圈
        for (i = 0; i < totalSteps; i++) {
            angle = i * angleStep + canvas.startAngle
            radians = angle * Math.PI / 180

            // 获取样本值
            sampleValue = (samples[i] || 0) * amplitude

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
        // 链接
        // for (i = 0; i < totalSteps; i++) {
        //     angle = i * angleStep + canvas.startAngle
        //     radians = angle * Math.PI / 180

        //     // 获取样本值 samples是0~1的归一化数组
        //     sampleValue = (samples[i] || 0) * amplitude
        //     if(Math.abs(sampleValue) < amplitude * 0.1){
        //         continue
        //     }

        //     // 极坐标转笛卡尔坐标
        //     x = canvas.width/2 + Math.cos(radians) * (r - sampleValue)
        //     y = canvas.height/2 + Math.sin(radians) * (r - sampleValue)
        //     ctx.moveTo(x, y) // 路径起点

        //     // 极坐标转笛卡尔坐标
        //     x = canvas.width/2 + Math.cos(radians) * (r + sampleValue)
        //     y = canvas.height/2 + Math.sin(radians) * (r + sampleValue)
        //     ctx.lineTo(x, y)
        // }
        ctx.stroke()
    }

    //关联
    Connections {
        target: MediaPlayer
        function onBufferSampleChanged (){requestPaint()}
    }
}
