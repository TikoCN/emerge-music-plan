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
        dataLoader.loadEnable: true
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadData:(index) => {
                            let list = SQLData.getAlbumByKey(currentKey, CoreData.pageSize, index)
                            list.forEach(id => {gridModel.append({id: id})})
                            if (list.length !== CoreData.pageSize) load.loadIsFinish = true
                        }
    }
}


