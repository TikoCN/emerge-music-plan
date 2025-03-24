import QtQuick.Controls
import QtQuick
import Widget
import Tiko
import TikoAPI
Item{
    id: mainPage

    ScrollView {
        anchors.fill: parent

        Column {
            width: mainPage.width
            spacing: 20

            Item {
                width: mainPage.width
                height: alumbRandText.height + alumbRandList.height

                TikoTextLine {
                    id: alumbRandText
                    exSize: 9
                    opacity: 0.7
                    width: parent.width
                    text: qsTr("随机推荐专辑")
                }

                ListView {
                    id: alumbRandList
                    anchors.top: alumbRandText.bottom
                    anchors.topMargin: 6
                    height: 150 + TikoSeit.fontPixelSize * 3
                    width: parent.width
                    orientation: ListView.Horizontal
                    spacing: 10

                    model: ListModel{
                        id: alumbRandModel
                    }

                    delegate: CoreAlumbButton {
                        alumb: alumbData
                    }
                }
            }

            Item {
                width: mainPage.width
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
    }

    function buildRand() {
        for (var i=0; i<15; i++) {
            var line = Math.floor(Math.random() * Core.artistLineList.length)
            var list = Math.floor(Math.random() * Core.artistLineList[line].length)
            artistRandModel.append({artistData: Core.artistLineList[line][list]})

            line = Math.floor(Math.random() * Core.alumbLineList.length)
            list = Math.floor(Math.random() * Core.alumbLineList[line].length)
            alumbRandModel.append({alumbData: Core.alumbLineList[line][list]})
        }
    }
}
