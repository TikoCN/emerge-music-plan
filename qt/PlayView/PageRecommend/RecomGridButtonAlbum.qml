import QtQuick
import Tiko
import PlayView
import MediaerAPI

RecomGridButtonBase {
    text: qsTr("随机推荐专辑")

    delegateItem: GridButtonAlbum {
        model: AlbumModel {
            type: AlbumModel.Rand
        }
    }
}