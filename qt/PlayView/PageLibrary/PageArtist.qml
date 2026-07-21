import QtQuick
import Tiko
import MediaerAPI
import PlayView


PageButtonBase {

    onInitKeyList: {
        const list = SQLData.getArtistKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonArtist{
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadInitData: {
            let list = SQLData.getArtistByKey(currentKey, CoreData.pageSize, 0)
            appendList(list)
        }

        dataLoader.onLoadData:(index) => {
                       let list = SQLData.getArtistByKey(currentKey, CoreData.pageSize, index)
                       appendList(list)
                   }
    }
}


