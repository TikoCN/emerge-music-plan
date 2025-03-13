import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import TikoAPI
import Tiko

Item {
    id: root
    property int tableId//歌曲标题
    property int allMusic:  0
    property string showCover: "qrc:/image/default.jpg"

    Item{
        id: showView
        width: parent.width
        height: childrenRect.height

        //列表封面
        Image{
            id: tableCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: root.showCover
            sourceSize.width: width
            sourceSize.height: height
            width: 200
            height: 200
            cache: false
            asynchronous: true
        }

        //列表名字
        TikoTextLine{
            id: tableName
            anchors.left: tableCover.right
            anchors.leftMargin: 10
            anchors.top: tableCover.top
            text: MediaPlayer.tableList[tableId].name
            width: parent.width - tableCover.width
            height: tableCover.height - playButton.height
            exSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        //播放列表
        TikoButtonBorder{
            id: playButton
            anchors.left: tableName.left
            anchors.bottom: tableCover.bottom
            text: qsTr("播放")
            icon.source: "qrc:/image/play.png"
            width: 100
            onClicked: MediaPlayer.playMusic(tableId, 0)

        }

        //批量操作
        TikoButtonBorder{
            anchors.left: playButton.right
            anchors.leftMargin: 10
            anchors.bottom: tableCover.bottom
            text: qsTr("批量")
            icon.source: "qrc:/image/batch.png"
            width: 100
        }

        RowLayout{
            id: tableTool
            anchors.top: tableCover.bottom
            anchors.topMargin: 10
            anchors.left: tableCover.left
            anchors.leftMargin: 10
            spacing: 10

            //显示所有歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: MediaPlayer.tableList[tableId].showAllMusic()
                text: qsTr("歌曲") + allMusic.toString()
                iconSource: "qrc:/image/music.png"
            }

            //显示喜爱歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: MediaPlayer.tableList[tableId].showLove()
                text: qsTr("喜爱")
                iconSource: "qrc:/image/love.png"
            }

            //排序
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: sortMenu.open()
                text: qsTr("排序")
                iconSource: "qrc:/image/sort.png"

                TikoMenu{
                    id: sortMenu
                    width: parent.width
                    y: parent.height

                    TikoMenuItem{
                        text: qsTr("标题升序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(0);
                        icon.source: "qrc:/image/asc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("标题降序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(1);
                        icon.source: "qrc:/image/desc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("歌手升序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(2);
                        icon.source: "qrc:/image/asc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("歌手降序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(3);
                        icon.source: "qrc:/image/desc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("专辑升序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(4);
                        icon.source: "qrc:/image/asc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("专辑降序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(5);
                        icon.source: "qrc:/image/desc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("时长升序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(6);
                        icon.source: "qrc:/image/asc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("时长降序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(7);
                        icon.source: "qrc:/image/desc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("修改时间升序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(8);
                        icon.source: "qrc:/image/asc.png"
                    }
                    TikoMenuItem{
                        text: qsTr("修改时间降序");
                        onTriggered: MediaPlayer.tableList[tableId].sortMusic(9);
                        icon.source: "qrc:/image/desc.png"
                    }
                }
            }

            //搜索
            TikoButtonIcon{
                id: searchButton
                text: qsTr("搜索")
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
                input.onEditingFinished: closeWidthAnimation.start()
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

    ScrollView{
        width: parent.width
        height: parent.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        ScrollBar.horizontal.visible: false
        ScrollBar.vertical: TikoBar{}

        ListView{
            id: musicList
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            spacing: 15
            clip: true

            model: ListModel{
                id: musicModel
            }

            delegate: CoreMusicLine{
                width: musicList.width - 20
                tableId: table
                listId: music
                core: musicCore
            }
        }
    }

    Connections{
        target: MediaPlayer.tableList[root.tableId]
        function onRebuildShowMusic(){
            //重新建立条目
            musicModel.clear()
            for(let i=0; i<MediaPlayer.tableList[root.tableId].showMusics.length; i++){
                musicModel.append({music: i,
                                      table: root.tableId,
                                      musicCore: MediaPlayer.tableList[root.tableId].showMusics[i]})
            }
        }

        function onAddMusic(size){
            musicModel.clear()
            for(var i=0; i<size; i++){
                var music = MediaPlayer.tableList[root.tableId].showMusics.length - size + i
                musicModel.append({music: music,
                                      table: root.tableId,
                                      musicCore: MediaPlayer.tableList[root.tableId].showMusics[music]})
            }

            //调整列表展示信息
            if(MediaPlayer.tableList[tableId].showMusics.length !== 0){
                var coorId = MediaPlayer.tableList[root.tableId].getLastCoreId()
                root.showCover = "image://cover/file:" + coorId.toString()
                root.allMusic = MediaPlayer.tableList[root.tableId].musics.length
            }
        }
    }
}
