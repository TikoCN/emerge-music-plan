import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageButtonBase {

    onInitKeyList: {
        const list = SQLData.getAlbumKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonAlbum {
        loadEnable: true
        autoHeightEnable: false
        flow: GridView.LeftToRight

        onLoadData:(index, pageSize) => {
                       let list = SQLData.getAlbumByKey(currentKey, pageSize, index)
                       toModel(list)

                       if (list.length !== pageSize) loadIsFinish = true
                   }
    }
}


