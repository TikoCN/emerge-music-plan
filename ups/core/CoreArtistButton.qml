import QtQuick
import Tiko
import MediaerAPI
import DataCore

Item{
    id: artistButton
    width: r + space * 2
    height: r + space * 2

    property int r: 160
    property int space: 5
    property int artistId: -1
    property ArtistData artist: visible ? Core.getArtist(artistId) : null

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
        onClicked: CoreData.mainTurnArtistPlayer(artist)
        hoverEnabled: true

        Item {
            id: showItem
            height: childrenRect.height
            x: space
            y: space
            width: r

            TikoImageAuto {
                id: artistCover
                width: r
                height: r
                normalUrl: "qrc:/image/artist.png"
                loadUrl: "image://cover/artist:" + artistId.toString()
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
                text: artist !== null ? artist.name : qsTr("专辑名")
            }

            // 专辑时长
            TikoTextLine {
                id: timeLine
                anchors.top: textLine.bottom
                width: parent.width
                opacity: 0.4
                text: artist !== null ? artist.getStringTime() : qsTr("00:00")
            }
        }
    }
}
