import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI   
import Tiko
import PlayView
import DataType

Item {
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
    id: playerTable
    property int tableId: -1
    property TableData table: visible ? Core.getTable(tableId) : null
    property int allMusic: 0
========
    id: playerPlayList
    property int sort: -1
    property int playlistId: -1
    property string name: ""
    property var musicList: []
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml

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
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
            loadUrl: "image://cover/tableFile:" + tableId.toString()
========
            loadUrl: "image://cover/playlistFile?id=" +
                     playlistId.toString() +
                     "&radius=10"
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
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
            exSize: 20
            font.bold: true
        }

        // 列表信息
        TikoTextLine{
            id: playlistHelp
            anchors.left: playlistName.left
            anchors.top: playlistName.bottom
            anchors.topMargin: 10
            width: parent.width - playlistCover.width
            exSize: 5
        }

        //播放列表
        TikoButtonBorder{
            id: playButton
            anchors.left: playlistHelp.left
            anchors.bottom: playlistCover.bottom
            text: qsTr("播放")
            icon.source: "qrc:/image/play.png"
            width: 100
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
            onClicked: MediaPlayer.playMusic(tableId, 0)
========
            onClick: MediaPlayer.playMusic(playlistId, 0)
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
            backColor: TikoSeit.themeColor
            textColor: TikoSeit.transparentColor
        }

        //批量操作
        TikoButtonBorder{
            anchors.left: playButton.right
            anchors.leftMargin: 10
            anchors.bottom: playlistCover.bottom
            text: qsTr("批量")
            icon.source: "qrc:/image/batch.png"
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
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
                onClickLeft: table.showOrignMusic()
                text: qsTr("歌曲") + allMusic.toString()
========
                text: qsTr("歌曲") + musicList.length.toString()
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
                iconSource: "qrc:/image/music.png"
                onClickLeft: {
                    musicList = Core.playListShowAllMusic(playlistId)
                    build()
                }
            }

            //显示喜爱歌曲列表
            TikoButtonNormal{
                Layout.minimumWidth: 70
                text: qsTr("喜爱")
                iconSource: "qrc:/image/love.png"
                onClickLeft: {
                    musicList = Core.playListShowLoveMusic(playlistId)
                    build()
                }
            }

            //排序
            TikoButtonNormal{
                Layout.minimumWidth: 70
                onClickLeft: CoreData.openMenuTableSort(this, playerTable.tableId)
                text: qsTr("排序")
                iconSource: "qrc:/image/sort.png"
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
========

                Component {
                    id: sortMenuComponent
                    MenuPlayListSort {
                        playlistId: playerPlayList.playlistId
                        sort: playerPlayList.sort
                    }
                }

                function sort(){
                    if (sortMenuComponent.status === Component.Ready){
                        var menu = sortMenuComponent.createObject(this)
                        menu.popup()
                    }
                }
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
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
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
                input.onEditingFinished: {
                    table.showSearchMusic(input.text)
========
                onFinish: {
                    musicList = Core.playListShowSearchMusic(playlistId, input.text)
                    build()
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
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
        id: musicListView
        width: playerPlayList.width - 40
        height: playerPlayList.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: playerPlayList.left
        anchors.leftMargin: 5
        interactive: true
        spacing: 15
        clip: true


        ScrollBar.vertical: TikoBarV{
        }


        model: ListModel{
            id: musicModel
        }

        delegate: CoreMusicLine{
            width: musicListView.width - 20
            listId: musicListId
            playlistId: playerPlayList.playlistId
            musicId: inMusicId
            onPlayMusic: MediaPlayer.buildPlayingListByMusicList(musicList, listId)
        }
    }

<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
    onTableChanged: {
        //调整列表展示信息
        if(table === null) return
        var length = table.musicList.length
        if(length !== 0){
            var coorId = table.musicList[length - 1]
            playerTable.allMusic = length
            tableHelp.text = table.musicList.length.toString()+" "+qsTr("首歌曲") +"-"+
                    Base.timeToString(table.duraiton)+" "+qsTr("歌曲长度")
            build()
        }
========
    function setPlayListId(id){
        if (playlistId === id)
            return
        playlistId = id
        var json = Core.getPlayListJson(id)
        musicList = Base.stringToIntList(json.musicList)
        sort = json.sort

        //调整列表展示信息
        var duration = 0

        duration = json.duraiton
        playlistHelp.text = musicList.length.toString()+" "+qsTr("首歌曲") +"-"+
                Base.timeToString(duration)+" "+qsTr("歌曲长度")
        build()
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
    }

    function build () {
        musicModel.clear()
        for(var i=0; i<musicList.length; i++){
            musicModel.append({
                                  musicListId: i,
                                  inMusicId: musicList[i]
                              })
        }
    }

    Connections {
<<<<<<<< Updated upstream:PlayView/player/PlayerTable.qml
        target: table
        function onBuildShow(){
========
        target: Core
        function onBuildPlayListPlayer(){
>>>>>>>> Stashed changes:PlayView/player/ShowPagePlayList.qml
            build()
        }
    }
}
