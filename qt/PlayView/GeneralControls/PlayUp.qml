import QtQuick
import Tiko
import MediaerAPI

TikoButtonIconLittle {
    id: playUp
    icon.source: "qrc:/image/up.png"
    onClicked: MediaPlayer.playNext(-1)
    //text: qsTr("播放上一首歌曲")
}

