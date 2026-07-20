import QtQuick
import Tiko
import MediaerAPI

TikoButtonIcon {
    id: playDown
    icon.source: "qrc:/image/down.png"
    onClicked: MediaPlayer.playNext(1)
    level: 0
    //text: qsTr("播放下一首歌曲")
}
