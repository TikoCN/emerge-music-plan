import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageButtonBase {

    onInitKeyList: {
        const list = AlbumLibrary.getAlbumKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonAlbum {
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadInitData: {
            AlbumLibrary.model().loadByKey(currentKey, CoreData.pageSize, 0)
        }

        dataLoader.onLoadData:(index) => {
                            AlbumLibrary.model().loadMoreByKey(currentKey, CoreData.pageSize, index)
                        }
    }
}