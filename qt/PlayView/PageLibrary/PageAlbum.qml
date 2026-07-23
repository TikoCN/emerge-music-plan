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
    }
}