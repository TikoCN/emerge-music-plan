import QtQuick
import PlayView
import MediaerAPI
import Tiko

Canvas{
    id: canvas
    clip: true
    property color showColor: TikoSeit.theme.baseTheme.foregroundNormal

    onPaint: {
        const ctx = getContext("2d");
        // 清除画布
        ctx.clearRect(0, 0, width, height)
        const inList = MediaPlayer.allSamples
        const length = inList.length
        const w = Math.min(width/length, 3)

        ctx.fillStyle = showColor
        ctx.strokeStyle = showColor
        //上半部分方形
        for(let i=0; i<length && i<inList.length; i++){
            ctx.fillRect(i*(w + 1), height,
                        w, (-height/2 * inList[i]) - 1)
        }
    }

    Connections {
        target: MediaPlayer
        function onBufferSampleChanged (){requestPaint()}
    }
}
