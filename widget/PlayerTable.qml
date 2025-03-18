import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import TikoAPI
import Tiko

Item {
    id: playerTable
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
            source: playerTable.showCover
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
            text: Core.tableList[tableId].name
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
        width: playerTable.width
        height: playerTable.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: playerTable.left
        anchors.leftMargin: 5
        anchors.right: playerTable.right
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
                listId: musicListId
                music: musicCore
            }
        }
    }

    Connections{
        target: Core.tableList[playerTable.tableId]
        function onClearMusic(){
            musicModel.clear()
        }

        function onUpdateMusic(start, length){
            playerTable.updateMusic(start, length)
        }

        function onUpdateCover(){
            playerTable.updateCover()
        }
    }

    Component.onCompleted: {
        playerTable.updateCover()
        playerTable.updateMusic(0, Core.tableList[playerTable.tableId].showMusics.length)
    }

    function updateCover(){
        //调整列表展示信息
        if(Core.tableList[tableId].showMusics.length !== 0){
            var coorId = Core.tableList[playerTable.tableId].getLastCoreId()
            playerTable.showCover = "image://cover/file:" + coorId.toString()
            playerTable.allMusic = Core.tableList[playerTable.tableId].musics.length
        }
    }

    function updateMusic(start, length){
        for(var i=start; i<start+length; i++){
            musicModel.append({
                                  musicListId: i,
                                  table: playerTable.tableId,
                                  musicCore: Core.tableList[playerTable.tableId].showMusics[i]
                              })
        }
    }
}
