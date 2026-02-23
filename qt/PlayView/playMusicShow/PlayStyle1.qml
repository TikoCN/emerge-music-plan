import QtQuick
import PlayView
import Tiko
import MediaerAPI

Item{
    id: style
    property string artist: qsTr("标题")
    property string title: qsTr("标题")
    property string icon: ""
    property bool show: false

    onShowChanged: {
        if (show) {
            lrcShow.buildLrcList()
        } else {
            lrcShow.clearData()
        }
    }

    Item{
        id: leftShow
        width: style.width / 2
        height: style.height

        TikoImageAuto {
            id: cover
            anchors.horizontalCenter: leftShow.horizontalCenter
            y: leftShow.height * 0.1
            width: min
            height: min
            loadUrl: style.icon
            normalUrl: "qrc:/image/exe.png"
            property double min: Math.min(leftShow.height * 0.5, leftShow.width * 0.8)
        }

        TikoTextTitle {
            id: titleItem
            anchors.top: cover.bottom
            anchors.topMargin: TikoSeit.emphasizeMargins
            anchors.horizontalCenter: leftShow.horizontalCenter
            width: cover.width
            text: style.title
        }

        TikoTextSubtitle {
            id: artistItem
            width: cover.width
            anchors.top: titleItem.bottom
            anchors.topMargin: TikoSeit.normalMargins
            anchors.horizontalCenter: leftShow.horizontalCenter
            text: style.artist
        }

        Row {
            anchors.top: titleItem.top
            anchors.bottom: artistItem.bottom
            anchors.right: cover.right
            spacing: TikoSeit.emphasizeMargins

            TikoButtonIconLittle {
                id: playUp
                anchors.verticalCenter: parent.verticalCenter
                icon.source: "qrc:/image/up.png"
                onClicked: MediaPlayer.playNext(-1)
                //text: qsTr("播放上一首歌曲")
            }

            TikoButtonIconLittle {
                id: playControlButton
                anchors.verticalCenter: parent.verticalCenter
                icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/play.png"
                onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
                //text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
            }

            TikoButtonIconLittle {
                id: playDown
                anchors.verticalCenter: parent.verticalCenter
                icon.source: "qrc:/image/down.png"
                onClicked: MediaPlayer.playNext(1)
                //text: qsTr("播放下一首歌曲")
            }
        }

        TikoSliderH{
            id: playControl
            anchors.top: artistItem.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: leftShow.horizontalCenter
            width: cover.width
            height: 10
            from: 0
            to: MediaPlayer.player.duration
            value: MediaPlayer.player.position
            size: 1
            showColor: TikoSeit.theme.baseTheme.foregroundNormal
            onMoved: {MediaPlayer.player.setPosition(value)}
        }

        TikoTextSubtitle {
            id: nowTime
            anchors.top: playControl.bottom
            anchors.topMargin: 10
            anchors.left: playControl.left
            text: formatTime(time)
            property int time: MediaPlayer.player.position / 1000

            // 定义转换函数
            function formatTime(seconds) {
                const mins = Math.floor(seconds / 60);
                const secs = seconds % 60;
                // 补零：强制两位数
                return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
            }
        }

        TikoTextSubtitle {
            id: lastTime
            anchors.top: playControl.bottom
            anchors.topMargin: 10
            anchors.right: playControl.right
            text: formatTime(time)
            property int time: (MediaPlayer.player.duration - MediaPlayer.player.position) / 1000

            // 定义转换函数
            function formatTime(seconds) {
                const mins = Math.floor(seconds / 60);
                const secs = seconds % 60;
                // 补零：强制两位数
                return `${mins.toString().padStart(2, '0')}:${secs.toString().padStart(2, '0')}`;
            }
        }

        AudioVsualizationRect {
            id: canvas
            width: artistItem.width
            height: leftShow.height * 0.15
            anchors.top: playControl.bottom
            anchors.horizontalCenter: leftShow.horizontalCenter
        }
    }


    Item{
        id: rightShow
        anchors.left: leftShow.right
        width: style.width / 2
        height: style.height

        //滚动歌词
        ListViewLrc {
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
}
