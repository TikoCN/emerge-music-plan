import QtQuick
import Tiko
import MediaerAPI
import PlayView

CoreButtonGrid {
    text: qsTr("随机推荐歌曲")

    delegateItem: GridButtonMusic {
        onInitData: {
            let dataList = SQLData.getMusicRandList()
            toModel(dataList)
        }
    }
}
