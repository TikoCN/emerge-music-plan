import QtQuick
import Tiko
import MediaerAPI
import PlayView

CoreButtonGrid {
    text: qsTr("随机推荐歌手")

    delegateItem: GridButtonArtist {
        onInitData: {
            let dataList = SQLData.getArtistRandList()
            toModel(dataList)
        }
    }
}
