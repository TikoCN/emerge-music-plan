import QtQuick.Controls.Basic
import QtQuick
import MediaerAPI   
import Tiko
import PlayView

// 正在播放列表
Drawer {
    id: playingPlaylist
    edge: Qt.RightEdge
    modal: false

    Rectangle{
        id: background
        color: TikoSeit.theme.colorBgView
        anchors.fill: parent
    }

    TikoTextLine{
        id: playingPlaylistText
        text: qsTr("正在播放: ") + qsTr("暂无歌曲")
        y: 10
        width: parent.width - 20
        anchors.horizontalCenter: parent.horizontalCenter

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                playingPlaylistText.text = qsTr("正在播放: ") + MusicLibrary.getData(MediaPlayer.playingMusicId).title
            }
        }
    }

    ListViewMusic {
        id: musicList
        anchors.bottom: background.bottom
        anchors.top: playingPlaylistText.bottom
        anchors.right: background.right
        anchors.left: background.left
        anchors.margins: TikoSeit.emphasizeMargins
        clip: true
        isLittle: true
        onPlay: (musicId, listId) => {MediaPlayer.playMusicByListId(listId)}
        model: MusicModel {
            id: musicModel
            type: MusicModel.NowQueueModel
        }
    }

    //关联
    Connections{
        target: MediaPlayer

        function onMusicListBuild(){
            musicModel.clear()
        }
    }
}