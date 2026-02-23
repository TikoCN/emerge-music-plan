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

        canvas.startAngle += 1
        const angleStep = 3;// 每3度一个样本
        const totalSteps = 360 / angleStep;
        const amplitude = r * 0.1
        const samples = MediaPlayer.allSamples
        let angle = 0
        let radians = 0
        let i = 0
        let sampleValue = 0
        let x = 0, y = 0

        // 确保样本数量足够，不足时补零
        for (i = 0; i < totalSteps; i++) {
            angle = i * angleStep + canvas.startAngle;
            radians = angle * Math.PI / 180;

            // 获取样本值（假设samples是0~1的归一化数组）
            sampleValue = (samples[i] || 0) * amplitude; // 放大样本效果
            if(Math.abs(sampleValue) < amplitude * 0.1){
                sampleValue = 0
            }

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

    //关联
    Connections {
        target: CoreData
        function onAutoUpdateUI (){requestPaint()}
    }
}
