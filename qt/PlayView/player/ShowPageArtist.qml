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
        text: name
    }

    TikoTextLine {
        id: artistHelp
        width: parent.width - artistDataCover.width
        anchors.top: artistText.bottom
        anchors.topMargin: 10
        anchors.left: artistText.left
        opacity: 0.5
        text: qsTr("共 ") +
              (musicList.length.toString()) +
              qsTr(" 首歌曲，共计 ") +
              BaseTool.typeConversion.durationToTimeStringNoMax(duration)
    }

    // 音乐列表
    MusicListView {
        id: musicListView
        anchors.top: artistDataCover.bottom
        anchors.bottom: artistPlayer.bottom
        anchors.left: parent.left
        anchors.margins: 30
        width: parent.width - 60
    }

    Connections {
        target: DataActive
        function onBuildArtistPlayer(){
            build()
        }
    }

    onArtistIdChanged: build()

    function setArtistId(artistId){
        artistPlayer.artistId = artistId
    }

    function build(){
        const json = DataActive.getArtistJson(artistId);
        name = json.artist
        duration = json.duration
        musicList = BaseTool.typeConversion.stringToIntList(json.musicList)
        musicListView.listToModel(musicList)
    }
}
