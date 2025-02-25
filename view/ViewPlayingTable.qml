import QtQuick.Controls.Basic
import QtQuick
import MyAPI
import "../base"
import "../core"

Drawer {
    id: playingTable
    edge: Qt.RightEdge
    modal: false

    Rectangle{
        color:  Setting.backdropColor
        anchors.fill:  parent
    }

    MyAutoText{
        id: playingTableText
        text: qsTr("正在播放: ") + qsTr("暂无歌曲")
        y: 10
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                playingTableText.text = qsTr("正在播放: ") + MediaPlayer.playingCore.title
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

        delegate: Component{
            id: musicDelegate
            CoreMusicLine{
                width: musicList.width - 20
                tableId: -1
                listId: music
                core: MediaPlayer.musicList[music]
            }
        }
    }

    //关联
    Connections{
        target: MediaPlayer

        function onCppBuildPlayingTable(){
            playingTable.buildMusicLine()
        }

        function onCppMusicInsertPlayingTable(){
            playingTable.addNewMuiscLine(MediaPlayer.musicList.length - 1)
        }

        function onCppPlayingInsertMusic(musicId){
            playingTable.insertNewMuiscLine(musicId)
        }
    }

    //建立播放列表
    function buildMusicLine(){
        musicModel.clear()

        for(var i=0; i<MediaPlayer.musicList.length; i++){
            addNewMuiscLine(i)
        }
    }

    //插入新条目 musicId 音乐的列表id
    function addNewMuiscLine(musicId){
        musicModel.append({music: musicId})
    }

    //插入新条目 musicId 音乐的列表id
    function insertNewMuiscLine(musicId){
        //删除插入条目后的数据
        var length = musicModel.count
        var update = length - musicId
        if(update > 0){
            musicModel.remove(musicId, update)
        }

        for(var i=musicId; i<MediaPlayer.musicList.length; i++){
            addNewMuiscLine(i)
        }
    }

    function clearData(){
        musicModel.clear()
    }
}
