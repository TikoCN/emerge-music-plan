

Canvas{
    id: canvas
    clip: true

    property var sampleList: []
    onPaint: {
        var ctx = getContext("2d")
        // 清除画布
        ctx.clearRect(0, 0, width, height);
        var inList = MediaPlayer.allSamples
        var w = 10
        var length = width / w

        var mainColor = Qt.rgba(Setting.themeColor.r, Setting.themeColor.g, Setting.themeColor.b, 0.5)
        var baseColor = Qt.rgba(Setting.themeColor.r, Setting.themeColor.g, Setting.themeColor.b, 0.03)

        for(var i=0; i<length && i<inList.length; i++){
            //左到右
            ctx.fillStyle = mainColor
            ctx.fillRect(i*(w + 3), height,
                        w, -height*2/3 * inList[i] - 10)
            //右到左
            ctx.fillStyle = baseColor
            ctx.fillRect(width - i*(w + 3), height,
                        w, -height*2/3 * inList[i] - 10)
        }
    }

    Connections{
        target: MediaPlayer
        function onCppDrawLine(){
            canvas.requestPaint()
        }
    }
}
