import QtQuick
import QtQuick.Controls.Basic
import MediaerAPI
import Tiko
import PlayView

Canvas {
    id: drawLrcFixHeight
    height: 0

    property int lrcId: -1
    property int fontPixelSize: Setting.mainLrcFont.pixelSize
    property int addFontSize: 0
    property int playingPos: MediaPlayer.player.position
    property string fontFamily: Setting.mainLrcFont.family
    property var startList: []
    property var endList: []
    property var textList: []

    onLrcIdChanged: load()
    Component.onCompleted: load()
    onPlayingPosChanged: drawLrcFixHeight.requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        var width = drawLrcFixHeight.width
        var height = drawLrcFixHeight.height

        oneFontMetrics.text = textList.join("")
        const fonstSpace = 3
        const allWith = oneFontMetrics.advanceWidth + textList.length * fonstSpace
        // 清除画布
        let centerY = height / 2
        let space = 10
        let boreder = 20
        let startY = height / 2
        let startX = allWith >= width ? 0 : (width - allWith) / 2
        let playingPos = MediaPlayer.player.position
        let overF = 0
        const playColor = Setting.lrcPlayingColor
        const normalColor = Setting.lrcNormalColor

        ctx.clearRect(0, 0, width, height);
        ctx.font = "bold "+ oneFontMetrics.font.pixelSize.toString() +
                "px " + oneFontMetrics.font.family +
                ", 'Microsoft YaHei', SimSun, sans-serif"


        for (let i=0; i<textList.length; i++) {
            oneFontMetrics.text = textList[i]
            let length = oneFontMetrics.advanceWidth + fonstSpace
            if(playingPos < startList[i]){
                overF = 0
                ctx.fillStyle = drawLrcFixHeight.normalColor;
            }
            else if(playingPos >= endList[i]){
                overF = 1
                ctx.fillStyle = drawLrcFixHeight.playingColor;
            }
            else{
                overF = (playingPos - startList[i]) /
                        (endList[i] - startList[i])
                overF = overF || 0
                // 创建线性渐变（从左到右）
                let gradient = ctx.createLinearGradient(startX, startY, startX + length - 3, startY);

                // 添加颜色停止点（0~1 范围）
                gradient.addColorStop(0, playColor);
                gradient.addColorStop(overF, playColor);
                gradient.addColorStop(overF, normalColor);
                gradient.addColorStop(1, normalColor);

                // 设置填充样式为渐变
                ctx.fillStyle = gradient;
            }

            ctx.fillText(textList[i], startX, startY - overF * 3)
            startX += length
        }
    }

    TextMetrics{
        id: oneFontMetrics
        font.family: fontFamily
        font.pixelSize: fontPixelSize + addFontSize
    }

    function load () {
        var json = MediaPlayer.getLrcJsonObject(lrcId)
        startList = BaseTool.typeConversion.stringToLongList(json.startList)
        endList = BaseTool.typeConversion.stringToLongList(json.endList)
        textList = BaseTool.typeConversion.stringToStringList(json.textList)
        if (textList.length === 0 || textList.join("") === "") {
            textList = ["♪♪♪"]
        }
    }


    Connections{
        target: MediaPlayer
        function onPlayingLrcIdChange(playingLrcId){
            lrcId = playingLrcId
        }
    }
}
