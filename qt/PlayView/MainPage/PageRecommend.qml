import QtQuick.Controls
import QtQuick
import PlayView
import Tiko
import MediaerAPI

ScrollView {
    id: mainPage
    ScrollBar.horizontal.visible: false
    ScrollBar.vertical: TikoBarV {
    }

    Column {
        id: showColumn
        width: mainPage.width - 80
        spacing: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.margins: 20

        RecomGridButtonMusic {
            id: recomMusic
            width: showColumn.width
            height: childrenRect.height
        }

        // 随机推荐专辑
        RecomGridButtonAlbum {
            id: recomAlbum
            width: showColumn.width
            height: childrenRect.height
        }

        RecomMusicNew {
            id: recomMusicNew
            width: showColumn.width
            height: childrenRect.height
        }

        RecomGridButtonArtist {
            id: recomArtist
            width: showColumn.width
            height: childrenRect.height
        }

        RecomMusicPlay {
            id: recomMusicPlay
            width: showColumn.width
            height: childrenRect.height
        }
    }

    function buildRand() {
        recomMusicNew.build()
        recomMusicPlay.build()
    }
}
