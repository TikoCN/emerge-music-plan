import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import ControlAPI   
import Tiko
import Widget
import DataCore

Item {
    id: playerTable
    property int tableId
    property TableData table: Core.tableList[tableId]
    property int allMusic:  0
    property string showCover: "qrc:/image/default.png"

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
            loadUrl: playerTable.showCover
            width: 200
            height: 200
        }

        //列表名字
        TikoTextLine{
            id: tableName
            anchors.left: tableCover.right
            anchors.leftMargin: 10
            anchors.top: tableCover.top
            text: table.name
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
            text: table.name
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
            table: playerTable.table
            listId: musicListId
            music: musicCore
        }
    }

    Connections{
        target: table
        function onBuildShow(){
            musicModel.clear()
            playerTable.updateMusic(0, table.showMusics.length)
        }

        function onMusicAppend(start, length){
            playerTable.appendMusic(start, length)
        }

        function onUpdateCover(){
            playerTable.updateCover()
        }
    }

    Component.onCompleted: {
        playerTable.updateCover()
        playerTable.appendMusic(0, table.showMusics.length)
    }

    function updateCover(){
        //调整列表展示信息
        if(table.showMusics.length !== 0){
            var coorId = table.getLastCoreId()
            playerTable.showCover = "image://cover/file:" + coorId.toString()
            playerTable.allMusic = table.musics.length
            var allTime = 0
            for (var i=0; i<table.musics.length; i++) {
                allTime += table.musics[i].endTime
            }
            tableHelp.text = table.musics.length.toString()+" "+qsTr("首歌曲") +"-"+
                    Base.timeToString(allTime)+" "+qsTr("歌曲长度")
        }
    }

    function appendMusic(start, length){
        for(var i=start; i<start+length; i++){
            musicModel.append({
                                  musicListId: i,
                                  musicCore: table.showMusics[i]
                              })
        }
    }
}
