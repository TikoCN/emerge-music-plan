import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageButtonBase {

    onInitKeyList: {
        const list = SQLData.getMusicKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonMusic {
        autoHeightEnable: false
        flow: GridView.LeftToRight

        dataLoader.onLoadInitData: {
            let list = SQLData.getMusicByKey(currentKey, CoreData.pageSize, 0)
            appendList(list)
        }

        dataLoader.onLoadData:(index) => {
                       let list = SQLData.getMusicByKey(currentKey, CoreData.pageSize, index)
                       appendList(list)
                   }
    }
}
