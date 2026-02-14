import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView
import DataType

Item {
    id: playerPlayList
    property int sort: -1
    property int duration: -1
    property int length: 0
    property string name: ""
    property string search: ""
    property alias musicList: musicListView

    Item{
        id: showView
        width: parent.width
        height: childrenRect.height

        //列表封面
        AutoCoverImage {
            id: playlistCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            normalUrl: "qrc:/image/default.png"
            loadUrl: "image://cover/playlistFile?id=" +
                     playlistId.toString() +
                     "&radius=10"
            width: 200
            height: 200
            extraLoadFlag: playlistId !== -1
        }

        //列表名字
        TikoTextLine{
            id: playlistName
            anchors.left: playlistCover.right
            anchors.leftMargin: 10
            anchors.top: playlistCover.top
            text: name
            width: parent.width - playlistCover.width
            font.bold: true
        }

        // 列表信息
        TikoTextLine{
            id: playlistHelp
            anchors.left: playlistName.left
            anchors.top: playlistName.bottom
            anchors.topMargin: 10
            width: parent.width - playlistCover.width
            text: qsTr("包含 %1 首歌曲，时长为：%2").arg(length, BaseTool.typeConversion.durationToTimeStringNoMax(duration))
        }

        //播放列表
        TikoButtonBorder{
            id: playButton
            anchors.left: playlistHelp.left
            anchors.bottom: playlistCover.bottom
            textLine.text: qsTr("播放")
            width: 100
            onClicked: MediaPlayer.playMusic(playlistId, 0)
        }

        //批量操作
        TikoButtonBorder{
            anchors.left: playButton.right
            anchors.leftMargin: 10
            anchors.bottom: playlistCover.bottom
            textLine.text: qsTr("批量")
            width: 100
        }

        RowLayout{
            id: playlistTool
            anchors.top: playlistCover.bottom
            anchors.topMargin: 10
            anchors.left: playlistCover.left
            anchors.leftMargin: 10
            spacing: 10

            //显示所有歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                textLine.text: qsTr("歌曲") + musicList.length.toString()
                icon.source: "qrc:/image/music.png"
                onLeftClicked: {}
            }

            //显示喜爱歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                textLine.text: qsTr("喜爱")
                icon.source: "qrc:/image/love.png"
                onLeftClicked: {}
            }

            //排序
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onLeftClicked: sort()
                textLine.text: qsTr("排序")
                icon.source: "qrc:/image/sort.png"

                Component {
                    id: sortMenuComponent
                    MenuPlayListSort {
                        playlistId: playerPlayList.playlistId
                        sort: playerPlayList.sort
                        onNewSortChanged: {
                            playerPlayList.sort = newSort
                            musicListView.reset()
                        }
                    }
                }

                function sort(){
                    if (sortMenuComponent.status === Component.Ready){
                        const menu = sortMenuComponent.createObject(this);
                        menu.popup()
                    }
                }
            }

            //搜索
            TikoButtonIcon{
                id: searchButton
                icon.source: "qrc:/image/search.png"
                onClicked: rotAnimation.start()

                //旋转动画
                RotationAnimation{
                    id: rotAnimation
                    target: searchButton
                    from: 0
                    to: 360
                    duration: 300
                    onFinished: {
                        searchButton.visible = false
                        inputText.visible = true
                        widthAnimation.start()
                    }
                }

                PropertyAnimation{
                    id: widthAnimation
                    target: inputText
                    property: "Layout.maximumWidth"
                    from: 0
                    to: 200
                    duration: 500
                }
            }

            TikoTextInput{
                id: inputText
                Layout.maximumWidth: 0
                show.text: qsTr("搜索")
                onFinish: {
                    search = input.text
                    musicListView.reset()
                    closeWidthAnimation.start()
                }
                visible: false

                PropertyAnimation{
                    id: closeWidthAnimation
                    target: inputText
                    property: "Layout.maximumWidth"
                    from: 200
                    to: 0
                    duration: 500
                    onFinished: {
                        searchButton.visible = true
                        inputText.visible = false
                    }
                }
            }
        }
    }

    MusicListView{
        id: musicListView
        width: playerPlayList.width - 40
        height: playerPlayList.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: playerPlayList.left
        anchors.leftMargin: 5
    }
}
