import QtQuick
import Tiko
import MediaerAPI   
import PlayView
import DataType

Item{
<<<<<<<< Updated upstream:PlayView/player/PlayerAlbum.qml
    id: albumDataShow
    property AlbumData album: Core.getAlbum(albumId)
========
    id: albumPlayer
>>>>>>>> Stashed changes:PlayView/player/ShowPageAlbum.qml
    property int albumId: -1
    property string name: ""
    property int duration: 0
    property var musicList: []

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

<<<<<<<< Updated upstream:PlayView/player/PlayerAlbum.qml
    Image {
========
    AutoCoverImage {
>>>>>>>> Stashed changes:PlayView/player/ShowPageAlbum.qml
        id: albumDataCover
        width: 250
        height: 250
        sourceSize.width: width
        sourceSize.height: height
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 30
<<<<<<<< Updated upstream:PlayView/player/PlayerAlbum.qml
        source: albumId !== -1 ?
                    "image://cover/albumOnline:" + albumId.toString()
                  : "qrc:/image/album.png"
========
        normalUrl: "qrc:/image/album.png"
        loadUrl: "image://cover/albumOnline?id=" +
                 albumId.toString() +
                 "&radius=10"
>>>>>>>> Stashed changes:PlayView/player/ShowPageAlbum.qml
    }

    TikoTextLine {
        id: albumText
        width: parent.width - albumDataCover.width - 60
        anchors.left: albumDataCover.right
        anchors.leftMargin: 30
        anchors.top: albumDataCover.top
        exSize: 20
        text: name
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
              (musicList.length.toString()) +
              qsTr(" 首歌曲，共计 ") +
              Base.timeToString(duration)
    }

    // 音乐列表
    ListView {
        id: musicListView
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
            width: musicListView.width
            listId: musicListId
            musicId: inMusicId
            onPlayMusic: MediaPlayer.buildPlayingListByMusicList(musicList, listId)
        }
    }

<<<<<<<< Updated upstream:PlayView/player/PlayerAlbum.qml
    onAlbumChanged: {
        if (album === null )
            return

        albumMusicModel.clear()
        for (var i=0; i<album.musicList.length; i++) {
========
    Connections {
        target: Core
        function onBuildAlbumPlayer(){
            build()
        }
    }
    onAlbumIdChanged: build()

    function setAlbumId(albumId){
        albumPlayer.albumId = albumId
    }

    function build(){
        var json = Core.getAlbumJson(albumId)
        name = json.album
        duration = json.duration
        musicList = Base.stringToIntList(json.musicList)

        albumMusicModel.clear()
        if (albumId === -1)
            return
        for (let i=0; i<musicList.length; i++) {
>>>>>>>> Stashed changes:PlayView/player/ShowPageAlbum.qml
            albumMusicModel.append({
                                       inMusicId: musicList[i],
                                       musicListId: i
                                   })
        }
    }
}
