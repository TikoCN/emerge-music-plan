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
    }
}