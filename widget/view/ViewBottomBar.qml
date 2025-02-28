import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQml
import TikoAPI
import Tiko

Item {
    //背景
    Rectangle{
        anchors.fill: parent
        color: Setting.transparentColor
        opacity: 0.05
    }

    Item{
        id: shwoView
        width: parent.width
        anchors.top: playPos.bottom
        anchors.topMargin: 10

        Image{
            id: cover
            x: 10
            width: 50
            height: 50
            sourceSize.width: width
            sourceSize.height: height
            source: "qrc:/image/cover.png"
        }

        TikoAutoText{
            id: title
            width: 200
            height: 30
            anchors.left: cover.right
            anchors.leftMargin: 10
            text: qsTr("标题")
        }

        TikoAutoText{
            id: artist
            width: 200
            height: 20
            anchors.left: cover.right
            anchors.leftMargin: 10
            anchors.top: title.bottom
            opacity:  0.5
            text: qsTr("作者")
        }

        //设置循环模式
        TikoUiButton{
            id: loopButton
            anchors.right: nextMusicUp.left
            anchors.rightMargin: 15
            text: qsTr("设置循环模式")

            TikoPopup{
                id: loopSelect
                visible: false
                y: loopButton.y-height
                width: 140
                height: 200

                contentItem:  Column {
                    spacing:  10

                    TikoUiButton {
                        id: loop0
                        text: qsTr("顺序播放")
                        icon.source: "qrc:/image/loop0.png"
                        onClicked: loopButton.setLoopType(0)
                        width: parent.width
                    }

                    TikoUiButton {
                        id: loop1
                        text: qsTr("随机循环")
                        icon.source: "qrc:/image/loop1.png"
                        onClicked: loopButton.setLoopType(1)
                        width: parent.width
                    }

                    TikoUiButton {
                        id: loop2
                        text: qsTr("单曲循环")
                        icon.source: "qrc:/image/loop2.png"
                        onClicked: loopButton.setLoopType(2)
                        width: parent.width
                    }
                }
            }
            onClicked: loopSelect.visible = true
            Component.onCompleted: setLoopType(MediaPlayer.loopType)

            function setLoopType(loopType){
                switch (loopType){
                case 0:
                    loopButton.icon.source = loop0.icon.source
                    break
                case 1:
                    loopButton.icon.source = loop1.icon.source
                    break
                case 2:
                    loopButton.icon.source = loop2.icon.source
                    break
                }
                MediaPlayer.loopType = loopType
            }
        }

        //播放上一首歌曲
        TikoUiButton{
            id: nextMusicUp
            anchors.right: playerControl.left
            anchors.rightMargin: 10
            icon.source: "qrc:/image/up.png"
            onClicked: MediaPlayer.playNext(-1)
            text: qsTr("播放上一首歌曲")
        }

        //播放 暂停按钮背景
        Rectangle{
            width: playerControl.width + 10
            height: playerControl.height + 10
            anchors.centerIn: playerControl
            color: Setting.themeColor
            radius: 15
        }
        //播放 暂停按钮
        TikoUiButton{
            x: parent.width/2 - width/2
            id: playerControl
            icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/play.png"
            onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
            text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
        }

        //下一首
        TikoUiButton{
            id: nextMusicDown
            anchors.left: playerControl.right
            anchors.leftMargin: 10
            icon.source: "qrc:/image/down.png"
            onClicked: MediaPlayer.playNext(1)
            text: qsTr("播放下一首歌曲")
        }

        //音量
        TikoUiButton{
            id: playVolume
            anchors.left: nextMusicDown.right
            anchors.leftMargin: 15
            icon.source: "qrc:/image/value.png"
            onClicked: volumePopup.open()
            text: qsTr("控制音量")

            TikoPopup{
                id: volumePopup
                y: - parent.height - height
                x: - width / 2 + parent.width / 2
                height: 200
                width: 35
                padding: 10

                contentItem: TikoVSlider{
                    from: 0
                    to: 100
                    value: MediaPlayer.audioOutput.volume * 100
                    onMoved: MediaPlayer.audioOutput.volume = value / 100
                    orientation: Qt.Vertical
                    radius: 6
                }
            }
        }

        //桌面歌词
        TikoUiButton{
            id: deskLrc
            anchors.right: playingListTabel.left
            anchors.rightMargin: 3
            icon.source: "qrc:/image/lrc.png"
            text: qsTr("桌面歌词")
            onClicked: {
                if(deskLrcTool === null){
                    deskLrcTool = deskLrcMake.createObject()
                    deskLrcTool.show()
                }
                else if(deskLrcTool.visible === true){
                    deskLrcTool.hide()
                }
                else{
                    deskLrcTool.show()
                }
            }
            property ToolDeskLrc deskLrcTool

            Component{
                id: deskLrcMake
                ToolDeskLrc{}
            }
        }

        //播放列表
        TikoUiButton{
            id: playingListTabel
            anchors.right: parent.right
            anchors.rightMargin: 10
            icon.source: "qrc:/image/list.png"
            text: qsTr("播放列表")

            onClicked: {
                if(playingTable.visible){
                    playingTable.close()
                }
                else{
                    playingTable.open()
                }
            }
        }
    }

    TikoHSlider{
        id: playPos
        width: parent.width
        height: 15
        from: 0
        to: MediaPlayer.player.duration
        value: MediaPlayer.player.position
        size: 1

        onMoved: {
            MediaPlayer.player.setPosition(value)
        }
    }

    //关联
    Connections{
        target: MediaPlayer.player
        function onSourceChanged(){
            artist.text = MediaPlayer.playingCore.artist
            title.text = MediaPlayer.playingCore.title
            cover.source = "image://cover/onLine:" + MediaPlayer.playingCore.coreId.toString()
        }
    }
}
