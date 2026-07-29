import QtQuick
import Tiko
import MediaerAPI
import PlayView


LibraryPage {

    onInitKeyList: {
        const list = ArtistLibrary.getKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonArtist{
        autoHeightEnable: false
        flow: GridView.LeftToRight
        model: ArtistModel {
            type: ArtistModel.Key
            key: currentKey
        }
    }
}