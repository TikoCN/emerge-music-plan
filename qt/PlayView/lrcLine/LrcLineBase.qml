import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import Tiko
import MediaerAPI

Item {
    id: lrcLine
    implicitHeight: 100
    implicitWidth: 100

    property font lrcFont: Qt.font({family: "Microsoft YaHei", pointSize: 12})
    property int lrcId: -1
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []
    property int wordStop: 0
    property var wordListX: []
    property var wordListY: []
    property var wordListWidth: []
    property bool isPlay: true
    property bool aloneLine: false
    property bool initPos: false

    property color playingColor: Qt.rgba(1, 1, 1, 1)
    property color normalColor: Qt.rgba(0.9, 0.9, 0.9, 1)
    property Canvas lrcCanvas: drawLrc

    onWidthChanged: {initPos = false; drawLrc.requestPaint()}
    onLrcIdChanged: {initPos = false; drawLrc.requestPaint()}
    onLrcFontChanged: {initPos = false; drawLrc.requestPaint()}

    Canvas {
        id: drawLrc
        visible: false
        anchors.fill: lrcLine
        renderStrategy: Canvas.Threaded

        onPaint: {
            if (initPos === false)
                updateHeight()

            const ctx = getContext("2d")
            // 清除画布
            ctx.clearRect(0, 0, width, height)
            ctx.font = fontToCssString(lrcFont)
            const lrc = lrcLine.textList
            let overF = 0.0 //超出当前字长
            let i = 0
            let wordId = 0

            let playingPos = MediaPlayer.playingPosition
            //计算主文本
            for(i=0; i<lrc.length; i++, wordId++){
                //计算字长

                if(playingPos < lrcLine.startList[i] || !isPlay){
                    overF = 0
                    ctx.fillStyle = lrcLine.normalColor;
                }
                else if(playingPos >= lrcLine.endList[i]){
                    overF = 1
                    ctx.fillStyle = lrcLine.playingColor;
                }
                else{
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
                    gradient.addColorStop(overF, lrcLine.playingColor);
                    gradient.addColorStop(overF, lrcLine.normalColor);
                    gradient.addColorStop(1, lrcLine.normalColor);

                    // 设置填充样式为渐变
                    ctx.fillStyle = gradient;
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
        }
    }

    MultiEffect {
        id: effct
        autoPaddingEnabled: true
        source: drawLrc
        anchors.fill: drawLrc
        blurEnabled: !(isPlay || mouseAreaItem.containsMouse)
        blurMax: 30
        blur: 0.5
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
        const maxH = maxFontMetrics.boundingRect.height
        const lrc = lrcLine.textList

        let i = 0
        let space = TikoSeit.emphasizeMargins
        let startX = space
        let startY = aloneLine ? height / 2:  maxH + space
        let line = 1
        let wordWidth = 0
        let wordId = 0

        wordListX = []
        wordListY = []
        wordListWidth = []
        wordStop = textList.length

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
            lrcLine.height = startY + space
        }

        initPos = true
    }

    function fontToCssString(font) {
        var style = font.bold ? "bold " : "";
        style += font.italic ? "italic " : "";
        // 根据需要添加其他修饰
        var size = font.pointSize + "pt";  // 或转换成 px
        var family = "'" + font.family + "'";
        return style + size + " " + family;
    }
}

