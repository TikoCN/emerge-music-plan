import QtQuick
import Tiko
import MediaerAPI
import PlayView
import DataType

Item{
    id: artistPlayer
    property int artistId: -1
    property string name: ""
    property int duration: 0
    property var musicList: []

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: CoreData.mainTurnArtistPage()
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

    AutoCoverImage {
        id: artistDataCover
        width: 250
        height: 250
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
        normalUrl: "qrc:/image/artist.png"
        loadUrl: "image://cover/artistOnline?id=" +
                 artistId.toString() +
                 "&radius=10"
    }

    TikoTextLine {
        id: artistText
        width: parent.width - artistDataCover.width - 60
        anchors.left: artistDataCover.right
        anchors.leftMargin: 30
        anchors.top: artistDataCover.top
        exSize: 20
        text: name
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
              (musicList.length.toString()) +
              qsTr(" 首歌曲，共计 ") +
              Base.timeToString(duration)
    }

    // 音乐列表
    ListView {
        id: musicListView
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
            width: musicListView.width
            listId: musicListId
            musicId: inMusicId
            onPlayMusic: MediaPlayer.buildPlayingListByMusicList(musicList, listId)
        }
    }

    Connections {
        target: Core
        function onBuildArtistPlayer(){
            build()
        }
    }

    onArtistIdChanged: build()

    function setArtistId(artistId){
        artistPlayer.artistId = artistId
    }

    function build(){
        var json = Core.getArtistJson(artistId)
        name = json.artist
        duration = json.duration
        musicList = Base.stringToIntList(json.musicList)

        artistMusicModel.clear()
        if (artistId === -1)
            return
        for (var i=0; i<musicList.length; i++) {
            artistMusicModel.append({
                                       inMusicId: artist.musicList[i],
                                       musicListId: i
                                   })
        }
    }
}
