import QtQuick
import Tiko
import MediaerAPI
import PlayView

RecomGridButtonBase {
    text: qsTr("随机推荐歌曲")

    delegateItem: GridButtonMusic {
        model: MusicModel {
            type: MusicModel.RandModel
        }
    }
}