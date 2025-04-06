import QtQuick.Controls
import QtQuick
import Ups
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

        RecomMusic {
            id: recomMusic
            width: showColumn.width
            height: childrenRect.height
        }

        // 随机推荐专辑
        RecomAlbum {
            id: recomAlbum
            width: showColumn.width
            height: childrenRect.height
        }

        RecomMusicNew {
            id: recomMusicNew
            width: showColumn.width
            height: childrenRect.height
        }

        RecomArtist {
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
        recomMusic.build()
        recomAlbum.build()
        recomMusicNew.build()
        recomArtist.build()
        recomMusicPlay.build()
    }
}
