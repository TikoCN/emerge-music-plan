import QtQuick
import Tiko
import DataType
import MediaerAPI
import PlayView

Item{
    id: albumButton
    width: r + space * 2
    height: showItem.height + space * 2

    property int r: 160
    property int space: 5
    property int albumId: -1
    property AlbumData album: visible ? Core.getAlbum(albumId) : null
    property int coverId: 0

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
        onClicked: CoreData.mainTurnAlbumPlayer(albumId)
        hoverEnabled: true

        Item {
            id: showItem
            height: childrenRect.height
            x: space
            y: space
            width: r

            TikoImageAuto {
                id: albumCover
                width: r
                height: r
                normalUrl: "qrc:/image/album.png"
                loadUrl: "image://cover/albumFile:" + albumId.toString()
            }

            // 播放按钮
            TikoButtonIcon {
                width: 30
                height: width
                radius: width / 2
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: albumCover.bottom
                anchors.left: albumCover.left
                anchors.margins: albumCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
            }

            // 菜单按钮
            TikoButtonIcon {
                width: 30
                height: width
                radius: width / 2
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: albumCover.top
                anchors.right: albumCover.right
                anchors.margins: albumCover.width * 0.05
                visible: mouse.containsMouse
                normal: 0.5
                hover: 1
                borderSize: 1.5
                autoColor: Setting.backdropColor
                onClicked: CoreData.openMenuAlbum(this, album)
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: albumCover.bottom
                anchors.topMargin: 10
                width: parent.width
                text: album !== null ? album.name : qsTr("专辑")
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                width: parent.width
                opacity: 0.4
                text: album !== null ? Base.timeToString(album.duration) : qsTr("00:00")
            }
        }
    }
}
