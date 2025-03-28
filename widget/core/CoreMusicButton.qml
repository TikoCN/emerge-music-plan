import QtQuick
import Tiko
import ControlAPI
import DataCore

Item {
    id: musicButton

    property MusicData music: null
    property int type: 0

    Rectangle {
        anchors.fill: parent
    }

    TikoImageAuto {
        id: musicCover
        normalUrl: "qrc:/image/exe.png"
        loadUrl: "image://cover/file:" + music.coreId.toString()
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 5
        width: parent.width - 10
        height: parent.height - 10
    }

    MouseArea{
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if (music === null) {
                return
            }

            MediaPlayer.appendPlayMusic(music)
            window.stackMusicPaly()
        }
    }

    // 播放按钮
    TikoButtonIcon {
        width: musicCover.width * 0.2
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
    }

    // 菜单按钮
    TikoButtonIcon {
        width: musicCover.width * 0.2
        height: width
        radius: width / 2
        icon.source: "qrc:/image/else.png"
        icon.width: width / 2
        icon.height: height / 2
        anchors.top: musicCover.top
        anchors.right: musicCover.right
        anchors.margins: musicCover.width * 0.05
        visible: mouse.containsMouse
        normal: 0.5
        hover: 0.1
        borderSize: 1.5
    }
}
