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
        const w = 3;
        const length = width / w;

        ctx.fillStyle = showColor
        ctx.strokeStyle = showColor
        //上半部分方形
        for(let i=0; i<length && i<inList.length; i++){
            ctx.fillRect(i*(w + 3), height,
                        w, -height/3 * inList[i])
        }
    }

    Connections {
        target: CoreData
        function onAutoUpdateUI (){requestPaint()}
    }
}
