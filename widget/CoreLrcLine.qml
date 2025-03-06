import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import TikoAPI
import Tiko

Item {
    id: coreLrcLine
    height: 100
    visible: !(text === " ")

    property font lrcFont: Setting.mainLrcFont
    property double pos: 0
    property string text: qsTr("歌词加载中")
    property bool isUse: true
    property bool isPlay: pos > 0
    property int lrcId: 0//歌词序号
    property color playingColor: "#ffffffff"
    property color normalColor: "#80ffffff"
    property var startList: []
    property var endList: []
    property real over: 0
    property real startTime: 0

    MultiEffect {
        autoPaddingEnabled: true
        source: lrcShow
        anchors.fill: lrcShow
        blurEnabled: !coreLrcLine.isPlay
        blurMax: 20
        blur: 0.5
    }

    Canvas{
        id: lrcShow
        anchors.fill: coreLrcLine
        visible: false
        onPaint: {
            var ctx = getContext("2d")
            // 清除画布
            ctx.clearRect(0, 0, width, height);
            ctx.font = "bold "+ lrcFont.pixelSize.toString() +"px 'Microsoft YaHei', SimSun, sans-serif"
            var maxH = maxFontMetrics.boundingRect.height
            var replace = new RegExp("[ ]*/[ ]*")
            var lrc = coreLrcLine.text.split(replace)
            var startX = 20
            var startY = 20+maxH
            var length = 0
            var overF = 0.0//超出当前字长

            //计算主文本
            for(var i=0; i<lrc[0].length; i++){
                //计算字长
                oneFontMetrics.text = lrc[0][i]
                length = oneFontMetrics.advanceWidth + 3
                //达到长度底部
                if(startX + length > coreLrcLine.width - 20){//超出长度
                    startX = 20
                    startY += maxH
                }

                if(coreLrcLine.over >= coreLrcLine.endList[i]){
                    overF = 1
                    ctx.fillStyle = coreLrcLine.playingColor;
                }
                else if(coreLrcLine.over < coreLrcLine.startList[i]){
                    overF = 0
                    ctx.fillStyle = coreLrcLine.normalColor;
                }
                else{
                    overF = (coreLrcLine.over - coreLrcLine.startList[i]) /
                            (coreLrcLine.endList[i] - coreLrcLine.startList[i])
                    overF = overF > 1 ? 1 : overF
                    overF = overF < 0 ? 0 : overF

                    // 创建线性渐变（从左到右）
                    var gradient = ctx.createLinearGradient(startX, startY, startX + length - 3, startY);

                    // 添加颜色停止点（0~1 范围）
                    gradient.addColorStop(0, coreLrcLine.playingColor);
                    gradient.addColorStop(overF, coreLrcLine.playingColor);
                    gradient.addColorStop(overF, coreLrcLine.normalColor);
                    gradient.addColorStop(1, coreLrcLine.normalColor);

                    // 设置填充样式为渐变
                    ctx.fillStyle = gradient;
                }

                ctx.fillText(lrc[0][i], startX, startY - overF * 3)
                startX += length
            }

            //计算辅助文本
            ctx.fillStyle = coreLrcLine.normalColor;
            for(var j=1; j<lrc.length; j++){
                startX = 20
                startY += maxFontMetrics.boundingRect.height
                for(i=0; i<lrc[j].length; i++){
                    //计算字长
                    oneFontMetrics.text = lrc[j][i]
                    length = oneFontMetrics.advanceWidth + 3
                    //达到长度底部
                    if(startX + length > coreLrcLine.width - 20){//超出长度
                        startX = 20
                        startY += maxH
                    }
                    ctx.fillText(lrc[j][i], startX, startY - overF * 3)
                    startX += length
                }
            }
        }
    }

    onLrcFontChanged: lrcShow.requestPaint()
    onPosChanged: {
        coreLrcLine.over = MediaPlayer.player.position - startTime
        lrcShow.requestPaint()
    }
    onWidthChanged: setHeight()
    Component.onCompleted: {setHeight()}

    TextMetrics{
        id: oneFontMetrics
        font: coreLrcLine.lrcFont
    }

    TextMetrics{
        id: maxFontMetrics
        font: coreLrcLine.lrcFont
        text: "Y高"
    }

    MouseArea{
        anchors.fill: parent
        propagateComposedEvents: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton

        onClicked:(mouse)=>{
                      //跳转到当前时间
                      if(mouse.button === Qt.LeftButton && isUse){
                          MediaPlayer.turnToLrc(coreLrcLine.lrcId)
                      }
                      mouse.accepted = false
                      console.log(text)
                  }
    }

    function setHeight(){
        var startX = 20
        var length = 0
        var line = 0
        var replace = new RegExp("[ ]*/[ ]*")
        var lrc = coreLrcLine.text.split(replace)

        for(var j=0; j<lrc.length; j++){
            startX = 20
            line++
            for(var i=0; i<lrc[j].length; i++){
                //计算字长
                oneFontMetrics.text = lrc[j][i]
                length = oneFontMetrics.advanceWidth + 3
                if(startX + length > coreLrcLine.width){
                    line++
                    startX = 20
                }
                startX += length
            }
        }

        coreLrcLine.height = 20 * 2 + line * maxFontMetrics.boundingRect.height
    }
}

