import QtQuick
import Widget
import Tiko
import TikoAPI

Item{
    id: style

    TikoAutoText{
        id: title
        x: style.width / 2 * 0.2
        width: style.width / 2 * 0.6
        height: style.height * 0.1
        font.bold: true
        exSize: 6
        text: qsTr("标题")
    }

    TikoAutoText{
        id: artist
        width: title.width
        height: style.height * 0.1
        anchors.top: title.bottom
        anchors.horizontalCenter: title.horizontalCenter
        exSize: 3
        text: qsTr("作者")
    }

    Image {
        id: cover
        cache: false
        width: style.height * 0.6
        height: style.height * 0.6
        anchors.top: artist.bottom
        anchors.horizontalCenter: title.horizontalCenter
        sourceSize.width: width
        sourceSize.height: height
        asynchronous: true
        source: "qrc:/image/cover.png"
    }

    //滚动歌词
    PlayerLrcTable{
        id: lrcShow
        x: style.width / 2
        y: style.height * 0.2
        width: style.width / 2 * 0.8
        height: style.height * 0.4
    }

    Canvas{
        id: canvas
        clip: true
        width: style.width / 2 * 0.9
        height: style.height * 0.2
        x: style.width * 0.5 + style.width * 0.5 * 0.05
        anchors.top: lrcShow.bottom
        onPaint: {
            var ctx = getContext("2d")
            // 清除画布
            ctx.clearRect(0, 0, width, height);
            var inList = MediaPlayer.allSamples
            var w = 10
            var length = width / w

            var mainColor = Qt.rgba(Setting.themeColor.r, Setting.themeColor.g, Setting.themeColor.b, 0.5)
            var baseColor = Qt.rgba(Setting.themeColor.r, Setting.themeColor.g, Setting.themeColor.b, 0.3)

            ctx.fillStyle = mainColor
            ctx.strokeStyle = mainColor
            //上半部分方形
            for(var i=0; i<length && i<inList.length; i++){
                ctx.fillRect(i*(w + 3), height/2,
                            w, -height/3 * inList[i])
            }
            ctx.beginPath()
            for(i=0; i<length && i<inList.length; i++){
                //下半部分条纹
                if(i===0){
                    ctx.moveTo(i*(w + 3) + w/2, height/2 + height/3 * inList[i])
                }
                else{
                    ctx.lineTo(i*(w + 3) + w/2, height/2 + height/3 * inList[i])
                }

                ctx.ellipse(i*(w + 3) + w/2 - 0.5, height/2 + height/3 * inList[i] * 1.5 - 0.5, 1, 1)
            }
            ctx.stroke()
        }
        Component.onCompleted: canvas.requestPaint()
    }


    //关联
    Connections{
        target: MediaPlayer
        function onCppDrawLine(){
            canvas.requestPaint()
        }
    }
    Connections{
        target:MediaPlayer.player
        function onSourceChanged(){
            artist.text = MediaPlayer.playingCore.artist
            title.text = MediaPlayer.playingCore.title
            cover.source = "image://cover/onLine:" + MediaPlayer.playingCore.coreId.toString()
        }
    }

    Component.onCompleted: {
        artist.text = MediaPlayer.playingCore.artist
        title.text = MediaPlayer.playingCore.title
        cover.source = "image://cover/onLine:" + MediaPlayer.playingCore.coreId.toString()
    }
}
