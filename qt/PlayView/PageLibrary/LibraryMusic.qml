import QtQuick
import Tiko
import MediaerAPI
import PlayView

LibraryPage {

    onInitKeyList: {
        const list = MusicLibrary.getKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonMusic {
        autoHeightEnable: false
        flow: GridView.FlowLeftToRight
        model: MusicModel {
            type: MusicModel.KeyModel
            key: currentKey
        }
    }
}
