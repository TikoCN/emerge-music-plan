import QtQuick.Controls.Basic
import QtQuick
import MediaerAPI   
import Tiko
import PlayView

Drawer {
    id: playingTable
    edge: Qt.RightEdge
    modal: false

    Rectangle{
        color: Setting.backdropColor
        anchors.fill: parent
    }

    TikoTextLine{
        id: playingTableText
        text: qsTr("正在播放: ") + qsTr("暂无歌曲")
        y: 10
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                playingTableText.text = qsTr("正在播放: ") + MediaPlayer.playingMusic.title
            }
        }
    }

    ListView{
        id: musicList
        width: parent.width - 20
        height: parent.height - playingTableText.height - 10
        anchors.top: playingTableText.bottom
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        clip: true

        model: ListModel{
            id: musicModel
        }

        delegate: CoreMusicLine{
            width: musicList.width - 20
            onPlay: MediaPlayer.playMusicList(listId)
            listId: musicListId
            musicId: MediaPlayer.musicList[musicListId].id
        }
    }

    //关联
    Connections{
        target: MediaPlayer

        function onMusicListBuild(){
            buildMusicLine()
        }

        function onMusicListAppend(start, length){
            appendMusic(start, length)
        }

        function onClearData(){
            clearData()
        }
    }

    //建立播放列表
    function buildMusicLine(){
        musicModel.clear()

        appendMusic(0, MediaPlayer.musicList.length)
    }

    //插入新条目 musicId 音乐的列表id
    function appendMusic(start, length){
        for (var i = start; i<length; i++) {
            musicModel.append({musicListId: i})
        }
    }

    function clearData(){
        musicModel.clear()
    }
}
