import QtQuick
import Tiko
import MediaerAPI
import PlayView

RecomGridButtonBase {
    text: qsTr("随机推荐歌曲")

    delegateItem: GridButtonMusic {
        dataLoader.onLoadData: {
            SQLData.getMusicRandList()
            dataLoader.loadIsFinish = true
        }
    }
}
