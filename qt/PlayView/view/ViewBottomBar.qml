import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQml
import MediaerAPI
import Tiko
import PlayView
import QtQuick.Effects

Item {
    //背景
    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.colorBgView
        radius: height * 0.3

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: TikoSeit.theme.colorBgHint
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }
    }

    Image {
        id: cover
        x: 10
        width: 50
        height: 50
        sourceSize.width: width
        sourceSize.height: height
        source: "qrc:/image/cover.png"
        anchors.verticalCenter: parent.verticalCenter

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (MediaPlayer.playingMusicId !== -1)
                    CoreData.windows.stackMusicPaly()
            }
        }
    }

    TikoTextLine {
        id: title
        width: 200
        height: 30
        anchors.left: cover.right
        anchors.leftMargin: 10
        anchors.top: cover.top
        text: qsTr("标题")
        level: 2
    }

    TikoTextLine {
        id: artist
        width: 200
        height: 20
        anchors.left: cover.right
        anchors.leftMargin: 10
        anchors.top: title.bottom
        opacity: 0.5
        text: qsTr("作者")
        level: 0
    }

    //设置循环模式
    TikoButtonIcon {
        id: loopButton
        anchors.right: nextMusicUp.left
        anchors.rightMargin: TikoSeit.emphasizeMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/loop" + MediaPlayer.loopType + ".svg"
        //text: qsTr("设置循环模式")
        onClicked: {
            var pop = loopSelectCom.createObject(null)
            pop.open()
        }

        Component {
            id: loopSelectCom
            TikoFloat {
                id: loopSelect
                parent: loopButton.Window.window.contentItem
                direction: "bottom"
                x: loopButton.mapToItem(loopButton.Window.window.contentItem, 0, 0).x + (loopButton.width - width) / 2
                y: loopButton.mapToItem(loopButton.Window.window.contentItem, 0, 0).y - height
                width: 140
                height: loopColumnLayout.height + TikoSeit.normalMargins * 2 + pointerSize

                ColumnLayout {
                    id: loopColumnLayout
                    spacing: TikoSeit.normalMargins

                    Repeater {
                        delegate: TikoButtonDefault {
                            id: loop0
                            textLine.text: modelData.text
                            icon.source: modelData.icon
                            onLeftClicked: {
                                MediaPlayer.loopType = modelData.type
                                loopSelect.close()
                            }
                            width: loopSelect.width - TikoSeit.normalMargins * 2
                        }

                        model: [
                            {text: qsTr("顺序播放"), type: 0, icon: "qrc:/image/loop0.svg"},
                            {text: qsTr("随机循环"), type: 1, icon: "qrc:/image/loop1.svg"},
                            {text: qsTr("单曲循环"), type: 2, icon: "qrc:/image/loop2.svg"}
                        ]
                    }
                }
            }
        }
    }

    //播放上一首歌曲
    TikoButtonIcon {
        id: nextMusicUp
        anchors.right: playerControl.left
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/up.svg"
        onClicked: MediaPlayer.playNext(-1)
        //text: qsTr("播放上一首歌曲")
        level: 0
    }

    //播放 暂停按钮
    TikoButtonIcon {
        x: parent.width / 2 - width / 2
        anchors.verticalCenter: parent.verticalCenter
        id: playerControl
        icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.svg" : "qrc:/image/play.svg"
        onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
        //text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
        level: 0
    }

    //下一首
    TikoButtonIcon {
        id: nextMusicDown
        anchors.left: playerControl.right
        anchors.leftMargin: TikoSeit.normalMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/down.svg"
        onClicked: MediaPlayer.playNext(1)
        //text: qsTr("播放下一首歌曲")
        level: 0
    }

    //音量
    TikoButtonIcon {
        id: playVolume
        anchors.left: nextMusicDown.right
        anchors.leftMargin: TikoSeit.emphasizeMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/value.svg"
        onClicked: {
            if (volumePopup === null) {
                volumePopup = volumePopupCom.createObject(null)
            }
            volumePopup.open()
        }
        level: 0
        //text: qsTr("控制音量")

        property TikoFloat volumePopup: null

        Component {
            id: volumePopupCom

            TikoFloat {
                parent: playVolume.Window.window.contentItem
                direction: "bottom"
                x: playVolume.mapToItem(playVolume.Window.window.contentItem, 0, 0).x + (playVolume.width - width) / 2
                y: playVolume.mapToItem(playVolume.Window.window.contentItem, 0, 0).y - height
                height: 200
                width: 72

                TikoSliderV {
                    anchors.fill: parent
                    anchors.margins: 8
                    from: 0
                    to: 100
                    value: MediaPlayer.audioOutput.volume * 100
                    onMoved: MediaPlayer.audioOutput.volume = value / 100
                    orientation: Qt.Vertical
                    radius: 6
                    size: 0.3
                }
            }
        }
    }

    //桌面歌词
    TikoButtonIcon {
        id: deskLrc
        anchors.right: playingListTabel.left
        anchors.rightMargin: TikoSeit.emphasizeMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/lrc.svg"
        level: 0
        checked: deskLrcWindow !== null
        //icon.dynamicState.isHighlight: (deskLrcWindow !== null)
        //text: qsTr("桌面歌词")
        onAnyClicked: {
            if (deskLrcWindow === null) {
                deskLrcWindow = deskLrcCom.createObject()
                deskLrcWindow.show()
            } else {
                deskLrcWindow.close()
                deskLrcWindow.destroy()
                deskLrcWindow = null
            }
        }

        property ToolDeskLrc deskLrcWindow: null
        Component {
            id: deskLrcCom
            ToolDeskLrc {
            }
        }
    }

    //播放列表
    TikoButtonIcon {
        id: playingListTabel
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.emphasizeMargins
        anchors.verticalCenter: parent.verticalCenter
        icon.source: "qrc:/image/list.svg"
        level: 0
        //text: qsTr("播放列表")

        onClicked: {
            if (playingPlaylist.visible) {
                playingPlaylist.close()
            } else {
                playingPlaylist.open()
            }
        }
    }

    TikoSliderH {
        id: playPos
        height: 5
        width: parent.width * 0.4
        from: 0
        to: MediaPlayer.player.duration
        value: MediaPlayer.player.position
        anchors.top: parent.top
        anchors.topMargin: TikoSeit.emphasizeMargins
        x: parent.width / 2 - width / 2
        radius: 5
        size: 1
        maxZoomIn: 4

        onMoved: {
            MediaPlayer.player.setPosition(value)
        }
    }

    //关联
    Connections {
        target: MediaPlayer.player

        function onSourceChanged() {
            if (MediaPlayer.playingMusic !== null) {
                var Json = MusicLibrary.getData(MediaPlayer.playingMusicId)
                artist.text = Json.artist
                title.text = Json.title
                cover.source = "image://cover/musicOnLine?id=" +
                    MediaPlayer.playingMusicId.toString() +
                    "&radius=10"
            }
        }
    }
}
