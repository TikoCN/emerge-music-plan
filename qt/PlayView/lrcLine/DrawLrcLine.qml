import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import MediaerAPI
import Tiko

Item {
    id: drawLrcLine
    height: 100

    property color playingColor: "#ffffffff"
    property color normalColor: "#80ffffff"
    property font lrcFont: Setting.lrcFont
    property int lrcId: -1
    property int maxH: maxFontMetrics.boundingRect.height
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []
    property bool isPlay: MediaPlayer.playingLrcId === lrcId
    property int playingPos: MediaPlayer.player.position

    onMaxHChanged: setHeight()
    onWidthChanged: setHeight()
    Component.onCompleted: setHeight()
    onPlayingPosChanged: {
        if (isPlay)
            lrcShow.requestPaint()
    }

    MultiEffect {
        id: effct
        autoPaddingEnabled: true
        source: lrcShow
        anchors.fill: lrcShow
        blurEnabled: !(isPlay || mouseArea.containsMouse)
        blurMax: 30
        blur: 0.5
    }

    Canvas{
        id: lrcShow
        anchors.fill: drawLrcLine
        visible: false
        onPaint: {
            const ctx = lrcShow.getContext("2d");
            const width = lrcShow.width;
            const height = lrcShow.height;
            // 清除画布
            ctx.clearRect(0, 0, width, height);
            ctx.font = "bold "+ lrcFont.pixelSize.toString() +"px 'Microsoft YaHei', SimSun, sans-serif"
            const maxH = drawLrcLine.maxH;
            const lrc = drawLrcLine.textList;
            let startX = 20;
            let startY = maxH * 1.8;
            let length = 0;
            let overF = 0.0;//超出当前字长
            let i =0;

            //计算主文本
            for(i=0; i<lrc.length; i++){
                //计算字长
                oneFontMetrics.text = lrc[i]
                length = oneFontMetrics.advanceWidth + 3
                //达到长度底部
                if(startX + length > drawLrcLine.width - 20){//超出长度
                    startX = 20
                    startY += maxH
                }

                if(playingPos < drawLrcLine.startList[i] || !isPlay){
                    overF = 0
                    ctx.fillStyle = drawLrcLine.normalColor;
                }
                else if(playingPos >= drawLrcLine.endList[i]){
                    overF = 1
                    ctx.fillStyle = drawLrcLine.playingColor;
                }
                else{
                    overF = (playingPos - drawLrcLine.startList[i]) /
                            (drawLrcLine.endList[i] - drawLrcLine.startList[i])
                    overF = overF || 0
                    // 创建线性渐变（从左到右）
                    const gradient = ctx.createLinearGradient(startX, startY, startX + length - 3, startY);

                    // 添加颜色停止点（0~1 范围）
                    gradient.addColorStop(0, drawLrcLine.playingColor);
                    gradient.addColorStop(overF, drawLrcLine.playingColor);
                    gradient.addColorStop(overF, drawLrcLine.normalColor);
                    gradient.addColorStop(1, drawLrcLine.normalColor);

                    // 设置填充样式为渐变
                    ctx.fillStyle = gradient;
                }

                ctx.fillText(lrc[i], startX, startY - overF * 3)
                startX += length
            }

            //计算辅助文本
            ctx.fillStyle = drawLrcLine.normalColor;

            for (let j=0; j<helpTextList.length; j++){
                const text = helpTextList[j];
                startX = 20
                startY += maxH

                for(i=0; i<text.length; i++){
                    //计算字长
                    oneFontMetrics.text = text[i]
                    length = oneFontMetrics.advanceWidth + 3
                    //达到长度底部
                    if(startX + length > drawLrcLine.width - 20){//超出长度
                        startX = 20
                        startY += maxH
                    }
                    ctx.fillText(text[i], startX, startY - overF * 3)
                    startX += length
                }
            }
        }
    }

    TextMetrics{
        id: oneFontMetrics
        font: drawLrcLine.lrcFont
    }

    TextMetrics{
        id: maxFontMetrics
        font: drawLrcLine.lrcFont
        text: "Y高"
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        propagateComposedEvents: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled: true

        onClicked:(mouse)=>{
                      //跳转到当前时间
                      if(mouse.button === Qt.LeftButton){
                          MediaPlayer.turnToLrc(drawLrcLine.lrcId)
                      }
                      mouse.accepted = false
                  }
    }

    function setHeight(){
        let i;
        let startX = 20;
        let length = 0;
        let line = 1;
        const lrc = drawLrcLine.textList;

        for(i = 0; i<lrc.length; i++){
            //计算字长
            oneFontMetrics.text = lrc[i]
            length = oneFontMetrics.advanceWidth + 3
            if(startX + length >= drawLrcLine.width - 20){
                line++
                startX = 20
            }
            startX += length
        }

        for (let j=0; j<helpTextList.length; j++){
            const helpLine = helpTextList[j];
            startX = 20
            line++

            for(i=0; i<helpLine.length; i++){
                    //计算字长
                    oneFontMetrics.text = helpLine[i]
                    length = oneFontMetrics.advanceWidth + 3
                    if(startX + length >=  drawLrcLine.width - 20){
                        line++
                        startX = 20
                    }
                    startX += length
            }
        }

        drawLrcLine.height = (line + 1.6) * maxFontMetrics.boundingRect.height
    }

    Connections{
        target: MediaPlayer
        function onPlayingLrcIdChange(playingLrcId){
            if (isPlay != (playingLrcId === lrcId)) {
                isPlay = playingLrcId === lrcId
                lrcShow.requestPaint()
            }
        }
    }
}
