import QtQuick
import Tiko
import MediaerAPI
import PlayView
import DataType

Item{
    id: artistDataShow
    property ArtistData artist: Core.getArtist(artistId)
    property int artistId: -1

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: CoreData.mainTurnartistPage()
    }

    // 专辑信息背景
    Rectangle {
        id: artistDataBack
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        width: parent.width - 60
        height: 270
        color: TikoSeit.backdropColor
        radius: 15
    }

    Image {
        id: artistDataCover
        width: 250
        height: 250
        sourceSize.width: width
        sourceSize.height: height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
        source: artistId !== -1 ?
                    "image://cover/artistOnline:" + artistId.toString()
                  : "qrc:/image/artist.png"
    }

    TikoTextLine {
        id: artistText
        width: parent.width - artistDataCover.width - 60
        anchors.left: artistDataCover.right
        anchors.leftMargin: 30
        anchors.top: artistDataCover.top
        exSize: 20
        text: artist !== null ? artist.name : qsTr("专辑")
    }

    TikoTextLine {
        id: artistHelp
        width: parent.width - artistDataCover.width
        anchors.top: artistText.bottom
        anchors.topMargin: 10
        anchors.left: artistText.left
        exSize: 5
        opacity: 0.5
        text: qsTr("共 ") +
              (artist !== null ? artist.musicList.length.toString() : "0") +
              qsTr(" 首歌曲，共计 ") +
              Base.timeToString(artist !== null ? artist.duration : 0)
    }

    // 音乐列表
    ListView {
        id: musicList
        anchors.top: artistDataCover.bottom
        anchors.left: parent.left
        anchors.margins: 30
        width: parent.width - 60
        height: parent.height - artistDataBack.height - 40
        clip: true

        model: ListModel {
            id: artistMusicModel
        }

        delegate: CoreMusicLine {
            width: musicList.width
            listId: musicListId
            musicId: inMusicId
            onPlay: MediaPlayer.buildMusicartist(artistId, listId)
        }
    }

    onArtistChanged: {
        if (artist === null )
            return

        artistMusicModel.clear()
        for (var i=0; i<artist.musicList.length; i++) {
            artistMusicModel.append({
                                       inMusicId: artist.musicList[i],
                                       musicListId: i
                                   })
        }
    }
}
