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
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadData:(index) => {
                            let list = SQLData.getAlbumByKey(currentKey, CoreData.pageSize, index)
                            appendList(list)
                        }
    }
}


