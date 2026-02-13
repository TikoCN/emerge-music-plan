import QtQuick
import Tiko
import PlayView
import MediaerAPI

CoreButtonGrid {
    text: qsTr("随机推荐专辑")

    delegateItem: GridButtonAlbum {
        onInitData: {
            let dataList = SQLData.getAlbumRandList();
            toModel(dataList)
        }
    }
}
