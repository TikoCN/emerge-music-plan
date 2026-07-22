import QtQuick
import Tiko
import MediaerAPI
import PlayView

RecomGridButtonBase {
    text: qsTr("随机推荐歌手")

    delegateItem: GridButtonArtist {
        dataLoader.onLoadInitData: {
            ArtistLibrary.model().loadRandList()
        }
    }
}