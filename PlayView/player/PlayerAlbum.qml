import QtQuick
import Tiko
import MediaerAPI   
import PlayView
import DataType

Item{
    id: albumDataShow
    property AlbumData album: Core.getAlbum(albumId)
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
        source: albumId !== -1 ?
                    "image://cover/albumOnline:" + albumId.toString()
                  : "qrc:/image/album.png"
    }

    TikoTextLine {
        id: albumText
        width: parent.width - albumDataCover.width - 60
        anchors.left: albumDataCover.right
        anchors.leftMargin: 30
        anchors.top: albumDataCover.top
        exSize: 20
        text: album !== null ? album.name : qsTr("专辑")
    }

    TikoTextLine {
        id: albumHelp
        width: parent.width - albumDataCover.width
        anchors.top: albumText.bottom
        anchors.topMargin: 10
        anchors.left: albumText.left
        exSize: 5
        opacity: 0.5
        text: qsTr("共 ") +
              (album !== null ? album.musicList.length.toString() : "0") +
              qsTr(" 首歌曲，共计 ") +
              Base.timeToString(album !== null ? album.duration : 0)
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
            id: albumMusicModel
        }

        delegate: CoreMusicLine {
            width: musicList.width
            listId: musicListId
            musicId: inMusicId
            onPlay: MediaPlayer.buildMusicAlbum(albumId, listId)
        }
    }

    onAlbumChanged: {
        if (album === null )
            return

        albumMusicModel.clear()
        for (var i=0; i<album.musicList.length; i++) {
            albumMusicModel.append({
                                       inMusicId: album.musicList[i],
                                       musicListId: i
                                   })
        }
    }
}
