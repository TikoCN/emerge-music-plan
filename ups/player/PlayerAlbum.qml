import QtQuick
import Tiko
import MediaerAPI   
import Ups
import DataCore

Item{
    id: albumDataShow
    property AlbumData album: null
    property int albumId: -1

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: CoreData.mainTurnAlbumPage()
    }

    // 专辑信息背景
    Rectangle {
        id: albumDataBack
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        width: parent.width - 60
        height: 270
        color: TikoSeit.backdropColor
        radius: 15
    }

    Image {
        id: albumDataCover
        width: 250
        height: 250
        sourceSize.width: width
        sourceSize.height: height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
    }

    TikoTextLine {
        id: albumText
        width: parent.width - albumDataCover.width - 60
        anchors.left: albumDataCover.right
        anchors.leftMargin: 30
        anchors.top: albumDataCover.top
        exSize: 20
    }

    TikoTextLine {
        id: albumHelp
        width: parent.width - albumDataCover.width
        anchors.top: albumText.bottom
        anchors.topMargin: 10
        anchors.left: albumText.left
        exSize: 5
        opacity: 0.5
    }

    // 音乐列表
    ListView {
        id: musicList
        anchors.top: albumDataCover.bottom
        anchors.left: parent.left
        anchors.margins: 30
        width: parent.width - 60
        height: parent.height - albumDataBack.height - 40
        clip: true

        model: ListModel {
            id: albumMusicList
        }

        delegate: CoreMusicLine {
            width: musicList.width
            listId: musicListId
            musicId: inMusicId
            onPlay: MediaPlayer.buildMusicAlbum(albumId, listId)
        }
    }

    function openAlbumData (albumId) {
        var allTime = 0
        albumDataShow.albumId = albumId
        album = Core.getAlbum(albumId)
        albumDataCover.source = "image://cover/albumFile:" +  album.id.toString()
        albumMusicList.clear()
        for (var i=0; i<album.musicList.length; i++) {
            albumMusicList.append({
                                      musicListId: i,
                                      inMusicId: album.musicList[i]
                                  })
            allTime += album.musicList[i].endTime
        }
        albumText.text = album.name
        albumHelp.text = album.musicList.length.toString()+" "+qsTr("首歌曲") +"-"
                +album.getStringTime()+" "+qsTr("歌曲长度")
    }
}
