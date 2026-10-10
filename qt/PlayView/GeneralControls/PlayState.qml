import QtQuick
import Tiko
import MediaerAPI

TikoButtonIcon {
    id: playControlButton
    icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.svg" : "qrc:/image/play.svg"
    onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
    //text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
    level: 0
}
