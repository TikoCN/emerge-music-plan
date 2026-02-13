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
        loadEnable: true
        autoHeightEnable: false
        flow: GridView.LeftToRight

        onLoadData:(index, pageSize) => {
                       let list = SQLData.getMusicByKey(currentKey, pageSize, index)
                       toModel(list)

                       if (list.length !== pageSize) loadIsFinish = true
                   }
    }
}
