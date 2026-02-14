import QtQuick
import Tiko
import MediaerAPI
import PlayView
import DataType

Item{
    id: albumPlayer
    property int albumId: -1
    property string name: ""
    property int duration: 0
    property var musicList: []

    TikoButtonIcon{
        y: -10
        icon.source: "qrc:/image/back.png"
        onClicked: CoreData.mainTurnAlbumPage()
    }

    AutoCoverImage {
        id: albumDataCover
        width: 250
        height: 250
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
        normalUrl: "qrc:/image/album.png"
        loadUrl: "image://cover/albumOnline?id=" +
                 albumId.toString() +
                 "&radius=10"
    }

    TikoTextLine {
        id: albumText
        width: parent.width - albumDataCover.width - 60
        anchors.left: albumDataCover.right
        anchors.leftMargin: 30
        anchors.top: albumDataCover.top
        text: name
    }

    TikoTextLine {
        id: albumHelp
        width: parent.width - albumDataCover.width
        anchors.top: albumText.bottom
        anchors.topMargin: 10
        anchors.left: albumText.left
        opacity: 0.5
        text: qsTr("共 ") +
              (musicList.length.toString()) +
              qsTr(" 首歌曲，共计 ") +
              BaseTool.typeConversion.durationToTimeStringNoMax(duration)
    }

    // 音乐列表
    MusicListView {
        id: musicListView
        anchors.top: albumDataCover.bottom
        anchors.bottom: albumPlayer.bottom
        anchors.left: parent.left
        anchors.margins: 30
        width: parent.width - 60
    }

    Connections {
        target: DataActive
        function onBuildAlbumPlayer(){
            build()
        }
    }

    onAlbumIdChanged: build()

    function setAlbumId(albumId){
        albumPlayer.albumId = albumId
    }

    function build(){
        const json = DataActive.getAlbumJson(albumId);
        name = json.album
        duration = json.duration
        musicList = BaseTool.typeConversion.stringToIntList(json.musicList)
        musicListView.listToModel(musicList)
    }
}
