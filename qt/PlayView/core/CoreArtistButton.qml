import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

Item{
    id: artistButton
    width: CoreData.cellItemWidth
    height: CoreData.cellItemHeight

    property int artistId: -1
    property int coverId: 0
    property string name: ""
    property int duration: -1

    Component.onCompleted: {
        var json = DataActive.getArtistJson(artistId)
        name = json.artist
        duration = json.duration
    }

    // 整体背景
    Rectangle {
        anchors.fill: parent
        color: Setting.transparentColor
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
                           CoreData.mainTurnArtistPlayer(artistId)
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
                id: artistCover
                width: CoreData.cellImageWidth
                height: CoreData.cellImageHeight
                normalUrl: "qrc:/image/artist.png"
                loadUrl: "image://cover/artistFile?id=" +
                         artistId.toString() +
                         "&radius=10"
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
                visible: mouseArea.containsMouse
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
                visible: mouseArea.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
                onClicked: createMenu(this)
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: artistCover.bottom
                anchors.topMargin: CoreData.cellItemSpace
                anchors.left: showItem.left
                width: parent.width
                text: name
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                anchors.left: showItem.left
                width: parent.width
                opacity: 0.4
                text: BaseTool.typeConversion.timeToString(duration)
            }
        }
    }

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
}
