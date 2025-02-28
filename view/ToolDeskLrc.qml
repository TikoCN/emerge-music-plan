import QtQuick
import QtQuick.Layouts
import TikoAPI
import "../core"
import Tiko

Window{
    id: deskLrcTool
    x: Setting.lrcTopPoint.x
    y: Setting.lrcTopPoint.y

    color:"#00000000"
    flags: Qt.Window | Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint
    title: qsTr("桌面歌词")
    width: lenth.boundingRect.width * 2
    height: tool.height + playingLine.height + playedLine.height + 60

    Component.onDestruction: {
        var mousePos = deskLrcToolArea.mapToGlobal(0, 0)
        Setting.lrcTopPoint.x = mousePos.x
        Setting.lrcTopPoint.y = mousePos.y
    }

    Rectangle{
        anchors.fill: parent
        color: Setting.transparentColor
        opacity: deskLrcToolArea.containsMouse ? 0.3 : 0
    }

    MouseArea{
        id: deskLrcToolArea
        hoverEnabled: true
        anchors.fill: parent
        onPressed: deskLrcTool.startSystemMove()

        RowLayout{
            id: tool
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            visible: deskLrcToolArea.containsMouse ? true : false
            height: 40

            TikoUiButton{
                icon.source: "qrc:/image/music.png"
                onClicked: window.show()
                text: qsTr("显示主窗口")
            }

            TikoUiButton{
                icon.source: "qrc:/image/size-.png"
                onClicked: Setting.deskFont.pixelSize--
                text: qsTr("字体减小")
            }

            TikoUiButton{
                icon.source: "qrc:/image/leftArrow.png"
                onClicked: MediaPlayer.player.position = MediaPlayer.player.position - 0.5 * 1000
                text: qsTr("快退")
            }

            //播放上一首歌曲
            TikoUiButton{
                icon.source: "qrc:/image/up.png"
                onClicked: MediaPlayer.playNext(-1)
                text: qsTr("播放上一首歌曲")
            }

            //播放 暂停按钮
            TikoUiButton{
                icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/playBlack.png"
                onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
                text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
            }

            //下一首
            TikoUiButton{
                icon.source: "qrc:/image/down.png"
                onClicked: MediaPlayer.playNext(1)
                text: qsTr("播放下一首歌曲")
            }

            TikoUiButton{
                icon.source: "qrc:/image/rightArrow.png"
                onClicked: MediaPlayer.player.position = MediaPlayer.player.position + 0.5 * 1000
                text: qsTr("快进")
            }

            TikoUiButton{
                icon.source: "qrc:/image/size+.png"
                onClicked: Setting.deskFont.pixelSize++
                text: qsTr("字体加大")
            }

            TikoUiButton{
                icon.source: "qrc:/image/close.png"
                onClicked: deskLrcTool.close()
                text: qsTr("隐藏歌词")
            }
        }

        TextMetrics{
            id: lenth
            text: qsTr("这用于计算桌面歌词长度文本")
            font.family: Setting.deskFont.family
            font.pixelSize: Setting.deskFont.pixelSize
            font.bold: true
        }

        CoreLrcLine{
            id: playingLine
            anchors.top: tool.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: 20
            lrcData: MediaPlayer.playingLrc
            lrcFont: Setting.deskFont
            width: parent.width - 40
            isUse: false
        }

        CoreLrcLine{
            id: playedLine
            anchors.top: playingLine.bottom
            anchors.topMargin: 20
            anchors.leftMargin: 20
            anchors.horizontalCenter: parent.horizontalCenter
            lrcData: MediaPlayer.playedLrc
            lrcFont: Setting.deskFont
            width: parent.width - 40
            isUse: false
        }
    }
}
