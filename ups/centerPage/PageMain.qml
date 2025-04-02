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

        Item {
            width: showColumn.width
            height: randMusicText.height + musicRandList.height + 10

            TikoTextLine {
                id: randMusicText
                exSize: 9
                opacity: 0.8
                width: parent.width
                text: qsTr("随机推荐歌曲")
            }

            ListView {
                id: musicRandList
                anchors.top: randMusicText.bottom
                anchors.topMargin: 6
                width: parent.width
                height: TikoSeit.fontPixelSize * 6 + 200
                orientation: ListView.Horizontal
                spacing: mainPage.height * 0.1

                model: ListModel {
                    id: musicRandModel
                }

                delegate: CoreMusicButton {
                    music: musicData
                    sizeR: 200
                }
            }
        }

        Item {
            width: showColumn.width
            height: albumRandText.height + albumRandList.height

            TikoTextLine {
                id: albumRandText
                exSize: 9
                opacity: 0.7
                width: parent.width
                text: qsTr("随机推荐专辑")
            }

            ListView {
                id: albumRandList
                anchors.top: albumRandText.bottom
                anchors.topMargin: 6
                height: 150 + TikoSeit.fontPixelSize * 6
                width: parent.width
                orientation: ListView.Horizontal
                spacing: 10

                model: ListModel{
                    id: albumRandModel
                }

                delegate: CoreAlbumButton {
                    album: albumData
                }
            }
        }

        Item {
            width: showColumn.width
            height: artistRandText.height + artistRandList.height

            TikoTextLine {
                exSize: 9
                opacity: 0.7
                id: artistRandText
                width: parent.width
                text: qsTr("随机推荐歌手")
            }

            ListView {
                id: artistRandList
                anchors.top: artistRandText.bottom
                anchors.topMargin: 6
                height: 160
                width: parent.width
                orientation: ListView.Horizontal
                spacing: 10

                model: ListModel{
                    id: artistRandModel
                }

                delegate: CoreArtistButton {
                    artist: artistData
                }
            }
        }

    }

    function buildRand() {

        var albumList = Core.getAlbumRandList()
        for (var i in albumList) {
            albumRandModel.append({albumData: albumList[i]})
        }

        var artistList = Core.getArtistRandList()
        for (i in artistList) {
            artistRandModel.append({artistData: artistList[i]})
        }

        var musicList = Core.getMusicRandList()
        for (i in musicList) {
            musicRandModel.append({musicData: musicList[i]})
        }
    }
}
