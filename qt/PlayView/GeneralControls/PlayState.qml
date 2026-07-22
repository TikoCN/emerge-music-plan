import QtQuick
import Tiko
import MediaerAPI

TikoButtonIcon {
    id: playControlButton
    icon.source: MediaPlayer.player.playing ? "qrc:/image/stop.png" : "qrc:/image/play.png"
    onClicked: MediaPlayer.player.playing ? MediaPlayer.player.pause() : MediaPlayer.player.play()
    //text: MediaPlayer.player.playing ? qsTr("暂停") : qsTr("播放")
    level: 0
}
