import QtQuick
import Tiko
import MediaerAPI
import DataType
import PlayView

Item{
    id: artistButton
    width: r + space * 2
    height: showItem.height + space * 2

    property int r: 160
    property int space: 5
    property int artistId: -1
<<<<<<< Updated upstream:PlayView/core/CoreArtistButton.qml
    property ArtistData artist: visible ? Core.getArtist(artistId) : null
=======
    property int coverId: 0
    property string name: ""
    property int duration: -1

    Component.onCompleted: {
        var json = Core.getArtistJson(artistId)
        name = json.artist
        duration = json.duration
    }
>>>>>>> Stashed changes:qt/PlayView/core/CoreArtistButton.qml

    // 整体背景
    Rectangle {
        anchors.fill: parent
        color: Setting.transparentColor
        radius: 10
        opacity: mouse.containsMouse ? 0.1 : 0
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: CoreData.mainTurnArtistPlayer(artistId)
        hoverEnabled: true

        Item {
            id: showItem
            height: childrenRect.height
            x: space
            y: space
            width: r

            AutoCoverImage {
                id: artistCover
                width: r
                height: r
                normalUrl: "qrc:/image/artist.png"
<<<<<<< Updated upstream:PlayView/core/CoreArtistButton.qml
                loadUrl: "image://cover/artistFile:" + artistId.toString()
=======
                loadUrl: "image://cover/artistFile?id=" +
                         artistId.toString() +
                         "&radius=10"
>>>>>>> Stashed changes:qt/PlayView/core/CoreArtistButton.qml
            }

            // 播放按钮
            TikoButtonIcon {
                width: 30
                height: width
                radius: width / 2
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: artistCover.bottom
                anchors.left: artistCover.left
                anchors.margins: artistCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
                onClicked: MediaPlayer.buildPlayingListByMusicList(musicList)
            }

            // 菜单按钮
            TikoButtonIcon {
                width: 30
                height: width
                radius: width / 2
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: artistCover.top
                anchors.right: artistCover.right
                anchors.margins: artistCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
                onClicked: CoreData.openMenuArtist(this, artist)
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: artistCover.bottom
                anchors.topMargin: 10
                width: parent.width
<<<<<<< Updated upstream:PlayView/core/CoreArtistButton.qml
                text: artist !== null ? artist.name : qsTr("专辑名")
=======
                text: name
>>>>>>> Stashed changes:qt/PlayView/core/CoreArtistButton.qml
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                width: parent.width
                opacity: 0.4
                text: Base.timeToString(duration)
            }
        }
    }
<<<<<<< Updated upstream:PlayView/core/CoreArtistButton.qml
=======

    Component {
        id: menuComponent
        MenuArtist {
            artistId: artistButton.artistId
            musicList: artistButton.musicList
            name: artistButton.name
        }
    }

    function createMenu(parent){
        if (menuComponent.status === Component.Ready){
            let menu = menuComponent.createObject(parent)
            menu.popup()
        }
        else
            console.log(menuComponent.errorString())
    }
>>>>>>> Stashed changes:qt/PlayView/core/CoreArtistButton.qml
}
