import QtQuick
import Tiko
import MediaerAPI
import PlayView

PageButtonBase {

    onInitKeyList: {
        const list = MusicLibrary.getMusicKeys()
        listToKeyModel(list)
    }

    delegateItem: GridButtonMusic {
        autoHeightEnable: false
        flow: GridView.LeftToRight
    }
}