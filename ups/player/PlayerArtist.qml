import QtQuick
import Tiko
import Ups
import MediaerAPI

Item{
    id: artistDataShow
    property var artist

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: mainView.turnToArtist()
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
    }

    TikoTextLine {
        id: artistText
        width: parent.width - artistDataCover.width - 60
        anchors.left: artistDataCover.right
        anchors.leftMargin: 30
        anchors.top: artistDataCover.top
        exSize: 20
    }

    TikoTextLine {
        id: artistHelp
        width: parent.width - artistDataCover.width
        anchors.top: artistText.bottom
        anchors.topMargin: 10
        anchors.left: artistText.left
        exSize: 5
        opacity: 0.5
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
            id: artistMusicList
        }

        delegate: CoreMusicLine {
            width: musicList.width
            listId: musicListId
            musicId: inMusicId
            type: 2
        }
    }


    function openArtistData (artist) {
        var allTime = 0
        artistDataCover.source = "image://cover/artist:" +  artist.id.toString()
        artistMusicList.clear()
        for (var i=0; i<artist.musicList.length; i++) {
            artistMusicList.append({
                                       musicListId: i,
                                       inMusicId: artist.musicList[i]
                                   })
            allTime += artist.musicList[i].endTime
        }
        artistText.text = artist.name
        artistHelp.text = artist.musicList.length.toString()+" "+qsTr("首歌曲") +"-"
               +artist.getStringTime()+" "+qsTr("歌曲长度")
        artistDataShow.artist = artist
    }
}
