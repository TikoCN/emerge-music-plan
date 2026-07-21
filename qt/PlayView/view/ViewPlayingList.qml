import QtQuick.Controls.Basic
import QtQuick
import MediaerAPI   
import Tiko
import PlayView

// 正在播放列表
Drawer {
    id: playingPlayList
    edge: Qt.RightEdge
    modal: false

    Rectangle{
        id: background
        color: TikoSeit.theme.colorBgView
        anchors.fill: parent
    }

    TikoTextLine{
        id: playingPlayListText
        text: qsTr("正在播放: ") + qsTr("暂无歌曲")
        y: 10
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                playingPlayListText.text = qsTr("正在播放: ") + MediaPlayer.playingMusicId.toString()
            }
        }
    }

    ListViewMusic {
        id: musicList
        anchors.bottom: background.bottom
        anchors.top: playingPlayListText.bottom
        anchors.right: background.right
        anchors.left: background.left
        anchors.margins: TikoSeit.emphasizeMargins
        clip: true
        isLittle: true
        onPlay: (musicId, listId) => {MediaPlayer.playMusicByListId(listId)}
        dataLoader.onLoadData: (index) => {
                                   let list = MediaPlayer.getMusicList(CoreData.pageSize, index)
                                   appendList(list)
                               }
    }

    //关联
    Connections{
        target: MediaPlayer

        function onMusicListBuild(){
            buildMusicLine()
        }
    }

    //建立播放列表
    function buildMusicLine(){
        musicList.reset()
        musicList.dataLoader.loadMore()
    }
}
