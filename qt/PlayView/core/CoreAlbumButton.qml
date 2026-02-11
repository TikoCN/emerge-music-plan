import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

Item{
    id: albumButton
    width: CoreData.cellItemWidth
    height: CoreData.cellItemHeight

    property int albumId: -1
    property int coverId: 0
    property int duration: 0
    property string name: ""
    property string key: ""
    property var musicList: []

    onAlbumIdChanged: {
        const json = DataActive.getAlbumJson(albumId);
        name = json.album
        duration = json.duration
        musicList = json.musicList
    }

    // 整体背景
    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.backgroundNormal
        radius: 10
        opacity: mouseArea.containsMouse ? 0.1 : 0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: (mouse) => {
                       switch(mouse.button){
                           case Qt.LeftButton:
                           CoreData.mainTurnAlbumPlayer(albumId)
                           break
                           case Qt.RightButton:
                           createMenu(this)
                           break
                       }
                   }

        //封面显示区
        Item {
            id: showItem
            height: childrenRect.height
            x: CoreData.cellItemSpace
            y: CoreData.cellItemSpace
            width: CoreData.cellImageWidth

            AutoCoverImage {
                id: albumCover
                width: CoreData.cellImageWidth
                height: CoreData.cellImageHeight
                normalUrl: "qrc:/image/album.png"
                loadUrl: "image://cover/albumFile?id=" +
                         albumId.toString() +
                         "&radius=10"
            }

            // 播放按钮
            TikoButtonIcon {
                width: 30
                height: width
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: albumCover.bottom
                anchors.left: albumCover.left
                anchors.margins: albumCover.width * 0.05
                visible: mouseArea.containsMouse
                onClicked: MediaPlayer.buildPlayingListByMusicList(musicList)
            }

            // 菜单按钮
            TikoButtonIcon {
                width: 30
                height: width
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: albumCover.top
                anchors.right: albumCover.right
                anchors.margins: albumCover.width * 0.05
                visible: mouseArea.containsMouse
                onClicked: createMenu(this)
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: albumCover.bottom
                anchors.left: albumCover.left
                width: parent.width
                text: name
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                anchors.left: albumCover.left
                width: parent.width
                opacity: 0.4
                text: BaseTool.typeConversion.timeToString(duration)
            }
        }
    }

    Component {
        id: menuComponent
        MenuAlbum {
            albumId: albumButton.albumId
            musicList: albumButton.musicList
            name: albumButton.name
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
}
