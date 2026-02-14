import QtQuick
import Tiko
import PlayView
import MediaerAPI

RecomGridButtonBase {
    text: qsTr("随机推荐专辑")

    delegateItem: GridButtonAlbum {
        dataLoader.onLoadData: {
            let list = SQLData.getAlbumRandList()
            dataLoader.loadIsFinish = true
        }
    }
}
