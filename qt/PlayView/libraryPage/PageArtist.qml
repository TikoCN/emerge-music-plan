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
        dataLoader.loadEnable: true
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadData:(index) => {
                       let list = SQLData.getArtistByKey(currentKey, CoreData.pageSize, index)
                       list.forEach(id => {gridModel.append({id: id})})
                       if (list.length !== CoreData.pageSize) load.loadIsFinish = true
                   }
    }
}


