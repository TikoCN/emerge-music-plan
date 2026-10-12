import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import Tiko
import MediaerAPI

Item {
    id: lrcLine
    implicitHeight: aloneLine ? maxFontMetrics.boundingRect.height + TikoSeit.emphasizeMargins * 2 : contentHeight
    implicitWidth: 100

    property font lrcFont: Qt.font({family: "Microsoft YaHei", pointSize: 12})
    property int lrcId: -1
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []
    property var wordListX: []
    property var wordListY: []
    property var wordListWidth: []
    property string fontcss: ""
    property bool isPlay: true
    property bool isEffect: false
    property bool aloneLine: false
    property bool initPos: false
    property bool hasPainted: false
    property real contentHeight: maxFontMetrics.boundingRect.height + TikoSeit.emphasizeMargins * 2

    property color playingColor: Qt.rgba(1, 1, 1, 1)
    property color normalColor: Qt.rgba(0.9, 0.9, 0.9, 1)
    property Canvas lrcCanvas: drawLrc

    onWidthChanged: {
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }
    onLrcIdChanged: {
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }
    onTextListChanged: {
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }
    onHelpTextListChanged: {
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }
    onLrcFontChanged: {
        fontcss = fontToCssString(lrcFont)
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }

    ListView.onReused: {
        initPos = false
        hasPainted = false
        updateHeight()
        drawLrc.requestPaint()
    }

    Component.onCompleted: {
        fontcss = fontToCssString(lrcFont)
        updateHeight()
        drawLrc.requestPaint()
    }

    Canvas {
        id: drawLrc
        visible: false
        anchors.fill: lrcLine

        onPaint: {
            if (initPos === false)
                updateHeight()

            if (initPos === false || width <= 0 || height <= 0) {
                return
            }

            const ctx = getContext("2d")
            // 清除画布
            ctx.clearRect(0, 0, width, height)
            ctx.font = fontcss
            const lrc = lrcLine.textList
            let overF = 0.0 //超出当前字长
            let i = 0
            let wordId = 0

            let playingPos = isPlay ? MediaPlayer.playingPosition : 0
            //计算主文本
            for(i=0; i<lrc.length; i++, wordId++){
                overF = 0
                ctx.fillStyle = lrcLine.normalColor;

                if (isPlay) {
                    if(playingPos >= lrcLine.endList[i]){
                        overF = 1
                        ctx.fillStyle = lrcLine.playingColor;
                    }
                    else if (playingPos > startList[i] && playingPos < endList[i]){
                        overF = (playingPos - lrcLine.startList[i]) /
                                (lrcLine.endList[i] - lrcLine.startList[i])
                        overF = overF || 0

                        // 创建线性渐变（从左到右）
                        const gradient = ctx.createLinearGradient(wordListX[wordId],
                                                                  wordListY[wordId],
                                                                  wordListX[wordId] + wordListWidth[wordId],
                                                                  wordListY[wordId]);

                        // 添加颜色停止点（0~1 范围）
                        gradient.addColorStop(0, lrcLine.playingColor);
                        gradient.addColorStop(Math.max(0, overF-0.05), lrcLine.playingColor);
                        gradient.addColorStop(overF, lrcLine.normalColor);
                        gradient.addColorStop(1, lrcLine.normalColor);

                        // 设置填充样式为渐变
                        ctx.fillStyle = gradient;
                    }
                }

                ctx.fillText(lrc[i], wordListX[wordId], wordListY[wordId] - overF * 3)
            }

            //计算辅助文本
            ctx.fillStyle = lrcLine.normalColor;

            for (let j=0; j<helpTextList.length; j++){
                const text = helpTextList[j];

                for(i=0; i<text.length; i++, wordId++){
                    ctx.fillText(text[i], wordListX[wordId], wordListY[wordId] - overF * 3)
                }
            }

            hasPainted = true
        }
    }

    MultiEffect {
        id: lyricEffect
        visible: hasPainted
        autoPaddingEnabled: false
        source: drawLrc
        anchors.fill: drawLrc
        blurEnabled: true
        blurMax: 30
        blur: isPlay || mouseAreaItem.containsMouse ? 0 : 0.5
    }

    TextMetrics{
        id: oneFontMetrics
        font: lrcLine.lrcFont
    }

    TextMetrics{
        id: maxFontMetrics
        font: lrcLine.lrcFont
        text: "Y高"
    }

    //关联
    Connections {
        target: CoreData
        function onAutoUpdateUI (){
            if(isPlay)
            drawLrc.requestPaint()
        }
    }

    function updateHeight(){
        if (width <= 0 || textList === undefined || helpTextList === undefined
            || maxFontMetrics.boundingRect.height <= 0) {
            return
        }

        const maxH = maxFontMetrics.boundingRect.height
        const lrc = lrcLine.textList

        let i = 0
        let space = TikoSeit.emphasizeMargins
        let startX = space
        let startY = aloneLine ? height / 2 + maxFontMetrics.boundingRect.height / 2 : maxH + space
        let line = 1
        let wordWidth = 0
        let wordId = 0

        wordListX = []
        wordListY = []
        wordListWidth = []

        let lineUp = () =>{
            startX = space
            startY += maxH
            line++
        }

        let setListPos = (list)=> {

            for(i = 0; i<list.length; i++, wordId++){
                //计算字长
                oneFontMetrics.text = list[i]
                wordWidth = oneFontMetrics.advanceWidth
                if(!aloneLine && startX + wordWidth >= lrcLine.width - space) {
                    lineUp()
                }
                wordListWidth.push(wordWidth)
                wordListX.push(startX)
                wordListY.push(startY)
                startX += wordWidth
            }
        }

        setListPos(lrc)

        for (let j = 0; j<helpTextList.length; j++){
            lineUp()
            setListPos(helpTextList[j])
        }

        if(aloneLine) {
            lrcLine.width = startX + space
        }
        else {
            contentHeight = startY + space
        }

        initPos = true
    }

    function fontToCssString(font) {
        var style = font.bold ? "bold " : "";
        style += font.italic ? "italic " : "";
        var pixelSize = font.pixelSize > 0 ? font.pixelSize : font.pointSize * 96 / 72;
        var size = pixelSize + "px";
        var family = "'" + font.family + "'";
        return style + size + " " + family;
    }
}

