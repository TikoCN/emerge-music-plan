import QtQuick
import Tiko
import MediaerAPI
import PlayView


PageButtonBase {

    onInitKeyList: {
        const list = ArtistLibrary.getArtistKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonArtist{
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadInitData: {
            ArtistLibrary.model().loadByKey(currentKey, CoreData.pageSize, 0)
        }

        dataLoader.onLoadData:(index) => {
                       ArtistLibrary.model().loadMoreByKey(currentKey, CoreData.pageSize, index)
                   }
    }
}