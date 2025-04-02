import QtQuick
import Tiko
import DataCore
import MediaerAPI

Item{
    id: albumButton
    width: 160
    height: showItem.height + 10

    property AlbumData album
    property int coverId: album.musicList[0].coreId

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
        onClicked: CoreData.mainTurnAlbumPlayer(album)
        hoverEnabled: true

        Item {
            id: showItem
            height: childrenRect.height
            x: 5
            y: 5
            width: parent.width - 10

            TikoImageAuto {
                id: albumCover
                width: parent.width
                height: parent.width
                normalUrl: "qrc:/image/album.png"
                loadUrl: "image://cover/file:" + albumButton.coverId.toString()
            }

            // 播放按钮
            TikoButtonIcon {
                width: albumCover.width * 0.2
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
                width: albumCover.width * 0.2
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

            // 图片上背景
            Rectangle {
                id: textBack
                width: albumCover.width * 0.7
                height: albumCover.height / 5
                anchors.bottom: albumCover.bottom
                anchors.right: albumCover.right
                topLeftRadius: 10
                bottomRightRadius: 10
                opacity: 0.6
                color: Setting.transparentColor
            }

            // 图片上文字
            TikoTextLine {
                anchors.fill: textBack
                font.pixelSize: 13
                font.bold: true
                color: Setting.backdropColor
                opacity: 0.7
                text: album.getArtist()
                horizontalAlignment: Text.AlignHCenter
            }

            // 专辑名
            TikoTextLine {
                id: textLine
                anchors.top: albumCover.bottom
                anchors.topMargin: 10
                width: parent.width
                text: album.name
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                width: parent.width
                opacity: 0.4
                text: album.getStringTime()
            }
        }
    }
}
