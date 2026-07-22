import QtQuick
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView
import QtQuick.Effects

Window{
    id: deskLrcTool
    x: Setting.lrcTopPoint.x
    y: Setting.lrcTopPoint.y
    color:"#00000000"
    flags: Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Tool
    title: qsTr("桌面歌词")
    width: (Setting.deskLrcFont.pixelSize * 20 > 600 ? Setting.deskLrcFont.pixelSize * 20: 600) + 40
    height: deskLrcToolArea.height + TikoSeit.emphasizeMargins * 2

    Component.onDestruction: {
        var mousePos = deskLrcToolArea.mapToGlobal(0, 0)
        Setting.lrcTopPoint.x = mousePos.x
        Setting.lrcTopPoint.y = mousePos.y
    }

    Rectangle {
        anchors.centerIn: parent
        width: parent.width - radius * 2
        height: parent.height - radius * 2
        color: TikoSeit.theme.colorBgView
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: TikoSeit.theme.colorBgHint
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }
        opacity: deskLrcToolArea.containsMouse ? 0.7 : 0
        radius: 10
    }

    MouseArea{
        id: deskLrcToolArea
        hoverEnabled: true
        width: parent.width
        height: tool.height + playingLine.height
        onPressed: {
            if (!tool.lock)
                deskLrcTool.startSystemMove()
        }

        Item {
            id: tool
            visible: deskLrcToolArea.containsMouse
            width: parent.width
            height: playBtn.height + TikoSeit.emphasizeMargins
            property bool lock: false

            //解锁按钮
            TikoButtonIcon{
                id: unlockBtn
                icon.source: "qrc:/image/unlock.png"
                onClicked: tool.lock = false
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: TikoSeit.emphasizeMargins
                visible: tool.lock
                //text: qsTr("解锁")
            }

            // 正常显示工具栏
            Item {
                id: toolLockItem
                anchors.fill: tool
                visible: !tool.lock

                TikoButtonIcon{
                    id: mainBtn
                    icon.source: "qrc:/image/music.png"
                    onClicked: window.show()
                    anchors.right: fontDscBtn.left
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("显示主窗口")
                }

                TikoButtonIcon{
                    id: fontDscBtn
                    icon.source: "qrc:/image/size-.png"
                    onClicked: Setting.deskLrcFont.pointSize--
                    anchors.right: backBtn.left
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("字体减小")
                }

                TikoButtonIcon{
                    id: backBtn
                    icon.source: "qrc:/image/leftArrow.png"
                    onClicked: MediaPlayer.player.position = MediaPlayer.player.position - 0.5 * 1000
                    anchors.right: upBtn.left
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("快退")
                }

                //播放上一首歌曲
                TikoButtonIcon{
                    id: upBtn
                    icon.source: "qrc:/image/up.png"
                    onClicked: MediaPlayer.playNext(-1)
                    anchors.right: playBtn.left
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("播放上一首歌曲")
                }

                //播放 暂停按钮
                TikoButtonIcon{
                    id: playBtn
                    icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/play.png"
                    onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.top: parent.top
                    anchors.topMargin: TikoSeit.emphasizeMargins
                    //text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
                }

                //下一首
                TikoButtonIcon{
                    id: downBtn
                    icon.source: "qrc:/image/down.png"
                    onClicked: MediaPlayer.playNext(1)
                    anchors.left: playBtn.right
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("播放下一首歌曲")
                }

                TikoButtonIcon{
                    id: forwordBtn
                    icon.source: "qrc:/image/rightArrow.png"
                    onClicked: MediaPlayer.player.position = MediaPlayer.player.position + 0.5 * 1000
                    anchors.left: downBtn.right
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("快进")
                }

                TikoButtonIcon{
                    id: fontAscBtn
                    icon.source: "qrc:/image/size+.png"
                    onClicked: Setting.deskLrcFont.pointSize++
                    anchors.left: forwordBtn.right
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("字体加大")
                }

                TikoButtonIcon{
                    id: lrcBtn
                    icon.source: "qrc:/image/close.png"
                    onClicked: deskLrcTool.destroy()
                    anchors.left: fontAscBtn.right
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("隐藏歌词")
                }

                //上锁按钮
                TikoButtonIcon{
                    id: lockBtn
                    icon.source: "qrc:/image/lock.png"
                    onClicked: tool.lock = true
                    anchors.left: lrcBtn.right
                    anchors.top: parent.top
                    anchors.margins: TikoSeit.emphasizeMargins
                    //text: qsTr("上锁")
                }
            }
        }

        LrcLineDesk {
            id: playingLine
            height: lrcFont.pixelSize * 1.5 + TikoSeit.emphasizeMargins * 2
            anchors.top: tool.bottom
            anchors.margins: TikoSeit.emphasizeMargins
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
