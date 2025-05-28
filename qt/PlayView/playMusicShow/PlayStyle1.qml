import QtQuick
import PlayView
import Tiko
import MediaerAPI

Item{
    id: style
    property color normalColor: Qt.rgba(Setting.backdropColor.r, Setting.backdropColor.g, Setting.backdropColor.b, 0.4)
    property color standColor: Qt.rgba(Setting.backdropColor.r, Setting.backdropColor.g, Setting.backdropColor.b, 0.9)

    Item{
        id: leftShow
        width: style.width / 2
        height: style.height

        Image {
            id: cover
            cache: false
            anchors.horizontalCenter: leftShow.horizontalCenter
            y: leftShow.height * 0.1
            width: min
            height: min
            sourceSize.width: width
            sourceSize.height: height
            asynchronous: true
            source: "qrc:/image/cover.png"
            property double min: Math.min(leftShow.height * 0.5, leftShow.width * 0.8)
        }

        TikoTextLine{
            id: title
            anchors.top: cover.bottom
            anchors.topMargin: 30
            anchors.horizontalCenter: leftShow.horizontalCenter
            width: cover.width
            font.bold: true
            color: style.standColor
            text: qsTr("标题")
        }

        TikoTextLine{
            id: artist
            width: cover.width
            anchors.top: title.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: leftShow.horizontalCenter
            text: qsTr("作者")
            color: style.normalColor
        }

        Row {
            anchors.top: title.top
            anchors.right: cover.right
            height: artist.y - title.y + artist.height
            width: height * 3 + 20
            spacing: 10

            TikoButtonIcon{
                id: playUp
                width: height
                height: parent.height
                icon.source: "qrc:/image/up.png"
                onClicked: MediaPlayer.playNext(-1)
                text: qsTr("播放上一首歌曲")
                autoColor: Setting.backdropColor
            }

            TikoButtonIcon{
                id: playControlButton
                width: height
                height: parent.height
                icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/play.png"
                onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
                text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
                autoColor: Setting.backdropColor
            }

            TikoButtonIcon{
                id: playDown
                width: height
                height: parent.height
                icon.source: "qrc:/image/down.png"
                onClicked: MediaPlayer.playNext(1)
                text: qsTr("播放下一首歌曲")
                autoColor: Setting.backdropColor
            }
        }

        TikoSliderH{
            id: playControl
            anchors.top: artist.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: leftShow.horizontalCenter
            width: cover.width
            height: 10
            from: 0
            to: MediaPlayer.player.duration
            value: MediaPlayer.player.position
            size: 1
            showColor: style.standColor
            lineColor: style.normalColor

            onMoved: {
                MediaPlayer.player.setPosition(value)
            }
        }

        TikoTextLine{
            id: nowTime
            anchors.top: playControl.bottom
            anchors.topMargin: 10
            anchors.left: playControl.left
            text: formatTime(time)
            color: style.normalColor
            property int time: MediaPlayer.player.position / 1000

            // 定义转换函数
            function formatTime(seconds) {
                const mins = Math.floor(seconds / 60);
                const secs = seconds % 60;
                // 补零：强制两位数
                return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
            }
        }

        TikoTextLine{
            id: lastTime
            anchors.top: playControl.bottom
            anchors.topMargin: 10
            anchors.right: playControl.right
            text: formatTime(time)
            color: style.normalColor
            property int time: (MediaPlayer.player.duration - MediaPlayer.player.position) / 1000

            // 定义转换函数
            function formatTime(seconds) {
                const mins = Math.floor(seconds / 60);
                const secs = seconds % 60;
                // 补零：强制两位数
                return `-${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
            }
        }

        Canvas{
            id: canvas
            clip: true
            width: artist.width
            height: leftShow.height * 0.15
            anchors.top: playControl.bottom
            anchors.horizontalCenter: leftShow.horizontalCenter

            onPaint: {
                var ctx = getContext("2d")
                // 清除画布
                ctx.clearRect(0, 0, width, height);
                var inList = MediaPlayer.allSamples
                var w = 3
                var length = width / w

                var mainColor = Qt.rgba(Setting.backdropColor.r, Setting.backdropColor.g, Setting.backdropColor.b, 0.5)

                ctx.fillStyle = mainColor
                ctx.strokeStyle = mainColor
                //上半部分方形
                for(var i=0; i<length && i<inList.length; i++){
                    ctx.fillRect(i*(w + 3), height,
                                w, -height/3 * inList[i])
                }
            }
            Component.onCompleted: canvas.requestPaint()
        }
    }


    Item{
        id: rightShow
        anchors.left: leftShow.right
        width: style.width / 2
        height: style.height

        //滚动歌词
        PlayerLrcTable{
            id: lrcShow
            y: rightShow.height * 0.1
            anchors.horizontalCenter: rightShow.horizontalCenter
            width: rightShow.width * 0.8
            height: rightShow.height * 0.8
        }
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
            artist.text = MediaPlayer.playingMusic.artist
            title.text = MediaPlayer.playingMusic.title
            cover.source = "image://cover/musicOnLine:"+ MediaPlayer.playingMusic.id.toString()
        }
    }

    Component.onCompleted: {
        artist.text = MediaPlayer.playingMusic.artist
        title.text = MediaPlayer.playingMusic.title
        cover.source = "image://cover/musicOnLine:"+ MediaPlayer.playingMusic.id.toString()
    }
}
