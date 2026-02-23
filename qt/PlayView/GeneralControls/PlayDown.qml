import QtQuick
import Tiko
import MediaerAPI

TikoButtonIconLittle {
    id: playDown
    icon.source: "qrc:/image/down.png"
    onClicked: MediaPlayer.playNext(1)
    //text: qsTr("播放下一首歌曲")
}
