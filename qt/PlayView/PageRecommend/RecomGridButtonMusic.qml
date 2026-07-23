import QtQuick
import Tiko
import MediaerAPI
import PlayView

RecomGridButtonBase {
    text: qsTr("随机推荐歌曲")

    delegateItem: GridButtonMusic {
        onResetRequested: {
            MusicLibrary.model().loadRandList()
        }
    }
}