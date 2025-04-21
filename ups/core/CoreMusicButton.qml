import QtQuick
import Tiko
import MediaerAPI
import DataCore
import Ups

Item {
    id: musicButton

    property int musicId: -1
    property int type: 0
    property MusicData music: musicButton.visible ? Core.getMusic(musicId) : null
    signal play()

    Rectangle {
        anchors.fill: parent
        color: Setting.transparentColor
        opacity: mouse.containsMouse ? 0.1 : 0
    }

    MouseArea{
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: play()

        TikoImageAuto {
            id: musicCover
            normalUrl: "qrc:/image/exe.png"
            loadUrl: "image://cover/musicFile:" + musicId.toString()
            loadFlag: visible && music !== null
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 5
            width: parent.width - 10
            height: parent.height - title.height - artist.height - 10
        }

        // 播放按钮
        TikoButtonIcon {
            width: 30
            height: width
            radius: width / 2
            icon.source: "qrc:/image/play.png"
            icon.width: width / 2
            icon.height: height / 2
            anchors.bottom: musicCover.bottom
            anchors.left: musicCover.left
            anchors.margins: musicCover.width * 0.05
            visible: mouse.containsMouse
            normal: 0.5
            hover: 0.1
            borderSize: 1.5
            autoColor: Setting.backdropColor
            onClicked: play()
        }

        // 菜单按钮
        TikoButtonIcon {
            width: 30
            height: width
            radius: width / 2
            icon.source: "qrc:/image/more.png"
            icon.width: width / 2
            icon.height: height / 2
            anchors.top: musicCover.top
            anchors.right: musicCover.right
            anchors.margins: musicCover.width * 0.05
            visible: mouse.containsMouse
            normal: 0.5
            hover: 0.1
            borderSize: 1.5
            autoColor: Setting.backdropColor
            onClicked: CoreData.openMenuMusic(this, music)
        }

        TikoTextLine {
            id: title
            text: music !== null ? music.title : qsTr("标题")
            width: parent.width
            anchors.top: musicCover.bottom
            anchors.left: parent.left
            anchors.margins: 10
        }

        TikoTextLine {
            id: artist
            text: music !== null ? music.artist : qsTr("作曲家")
            width: parent.width
            opacity: 0.7
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.margins: 10
        }
    }

}
