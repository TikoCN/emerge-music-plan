import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageButtonBase {

    onInitKeyList: {
        const list = MusicLibrary.getMusicKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonMusic {
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadInitData: {
            MusicLibrary.model().loadByKey(currentKey, CoreData.pageSize, 0)
        }

        dataLoader.onLoadData:(index) => {
                       MusicLibrary.model().loadMoreByKey(currentKey, CoreData.pageSize, index)
                   }
    }
}