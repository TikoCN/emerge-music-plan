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
        TikoImageAuto {
            id: tableCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            normalUrl: "qrc:/image/default.jpg"
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
            text: Core.tableList[tableId].name
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
            text: Core.tableList[tableId].name
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
                onClickLeft: Core.tableList[playerTable.tableId].showAllMusic()
                text: qsTr("歌曲") + allMusic.toString()
                iconSource: "qrc:/image/music.png"
            }

            //显示喜爱歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: Core.tableList[playerTable.tableId].showLove()
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

        ScrollBar.vertical: TikoBar{}
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
            var table = Core.tableList[playerTable.tableId]
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
