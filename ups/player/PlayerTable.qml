import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI   
import Tiko
import Ups
import DataCore

Item {
    id: playerTable
    property int tableId: -1
    property TableData table: visible ? Core.getTable(tableId) : null
    property int allMusic: 0

    Item{
        id: showView
        width: parent.width
        height: childrenRect.height

        //列表封面
        TikoImageAuto {
            id: tableCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            normalUrl: "qrc:/image/default.png"
            loadUrl: "image://cover/tableFile:" + tableId.toString()
            width: 200
            height: 200
            loadFlag: visible && table !== null && tableId !== -1
        }

        //列表名字
        TikoTextLine{
            id: tableName
            anchors.left: tableCover.right
            anchors.leftMargin: 10
            anchors.top: tableCover.top
            text: table !== null ? table.name : qsTr("列表名")
            width: parent.width - tableCover.width
            exSize: 20
            font.bold: true
        }

        // 列表信息
        TikoTextLine{
            id: tableHelp
            anchors.left: tableName.left
            anchors.top: tableName.bottom
            anchors.topMargin: 10
            width: parent.width - tableCover.width
            exSize: 5
        }

        //播放列表
        TikoButtonBorder{
            id: playButton
            anchors.left: tableHelp.left
            anchors.bottom: tableCover.bottom
            text: qsTr("播放")
            icon.source: "qrc:/image/play.png"
            width: 100
            onClicked: MediaPlayer.playMusic(tableId, 0)
            backColor: TikoSeit.themeColor
            textColor: TikoSeit.transparentColor
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
                onClickLeft: table.showAllMusic()
                text: qsTr("歌曲") + allMusic.toString()
                iconSource: "qrc:/image/music.png"
            }

            //显示喜爱歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: table.showLoveMusic()
                text: qsTr("喜爱")
                iconSource: "qrc:/image/love.png"
            }

            //排序
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: CoreData.openMenuTableSort(this, playerTable.tableId)
                text: qsTr("排序")
                iconSource: "qrc:/image/sort.png"
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
                input.onEditingFinished: {
                    table.showSearchMusic(input.text)
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

    ListView{
        id: musicList
        width: playerTable.width - 40
        height: playerTable.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: playerTable.left
        anchors.leftMargin: 5
        anchors.right: playerTable.right
        anchors.rightMargin: 5

        ScrollBar.vertical: TikoBarV{}
        spacing: 15
        clip: true

        model: ListModel{
            id: musicModel
        }

        delegate: CoreMusicLine{
            width: musicList.width - 20
            listId: musicListId
            musicId: inMusicId
            onPlay: MediaPlayer.buildMusicTable(tableId, listId)
        }
    }

    onTableChanged: {
        //调整列表展示信息
        if(table === null) return
        if(table.musicList.length !== 0){
            var coorId = table.getLastCoreId()
            playerTable.allMusic = table.musicList.length
            var allTime = 0
            tableHelp.text = table.musicList.length.toString()+" "+qsTr("首歌曲") +"-"+
                    Base.timeToString(allTime)+" "+qsTr("歌曲长度")
        }

        musicModel.clear()
        for(var i=0; i<table.musicList.length; i++){
            musicModel.append({
                                  musicListId: i,
                                  inMusicId: table.musicList[i]
                              })
        }
    }
}
