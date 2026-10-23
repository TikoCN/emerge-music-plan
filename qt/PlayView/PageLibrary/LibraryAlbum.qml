import QtQuick
import Tiko
import MediaerAPI
import PlayView

LibraryPage {

    onInitKeyList: {
        const list = AlbumLibrary.getKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonAlbum {
        autoHeightEnable: false
        flow: GridView.FlowLeftToRight
        model: AlbumModel {
            type: AlbumModel.KeyModel
            key: currentKey
        }
    }
}
