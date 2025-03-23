import QtQuick
import QtQuick.Controls
import Tiko
import TikoAPI
import Widget

Item {
    id: artistPage

    StackView {
        id: artistListShow
        initialItem: artistLineShow
        anchors.fill: parent

        Item{
            id: artistLineShow
            visible: false

            ListView {
                id: artistTurnButtonList
                orientation: ListView.Horizontal
                currentIndex: artistLineList.currentIndex
                width: parent.width - 60
                anchors.left: parent.left
                anchors.margins: 30
                spacing: 10
                highlightRangeMode: ListView.StrictlyEnforceRange
                preferredHighlightBegin: 0
                preferredHighlightEnd: 0

                model: ListModel {
                    id: artistTurnButtonModel
                }

                delegate: TikoTextLine {
                    text: lineText
                    height: 40
                    opacity: ListView.isCurrentItem ? 1 : 0.3
                    exSize: 3

                    MouseArea {
                        anchors.fill: parent
                        onClicked: artistLineShow.turnToLine(lineId)
                    }
                }
            }

            ListView {
                id: artistLineList
                anchors.left: parent.left
                anchors.top: artistTurnButtonList.bottom
                anchors.margins: 30
                width: parent.width - 60
                height: parent.height - artistTurnButtonList.height - 60
                highlightRangeMode: ListView.StrictlyEnforceRange
                preferredHighlightBegin: 0
                preferredHighlightEnd: 0

                ScrollBar.vertical: TikoBar{
                    id: artistLineListBar
                }
                clip: true

                model: ListModel{
                    id: artistModel
                }

                delegate: Item {
                    width: artistLineShow.width
                    height: artistLineText.height + artistShow.height

                    TikoTextLine {
                        id: artistLineText
                        text: lineText
                        width: parent.width
                        exSize: 10
                        color: TikoSeit.themeColor
                        opacity: parent.ListView.isCurrentItem ? 1 : 0.3
                    }

                    Grid {
                        id: artistShow
                        width: parent.width
                        columns: parent.width / (150 + 20)
                        spacing: 20
                        height: (leng / columns + 1) * ((150 + 20))
                        anchors.top: artistLineText.bottom
                        anchors.margins: 10

                        Repeater{
                            model: artistDataList

                            delegate: Item{
                                width: 150
                                height: artistCover.height + textLine.height + 10

                                TikoImageAuto {
                                    id: artistCover
                                    width: parent.width
                                    height: 150
                                    normalUrl: "qrc:/image/artist.png"
                                    loadUrl: "image://cover/file:" +  musicId.toString()
                                }

                                TikoTextLine {
                                    id: textLine
                                    anchors.top: artistCover.bottom
                                    anchors.topMargin: 10
                                    width: parent.width
                                    text: artist.name
                                }

                                MouseArea {
                                    anchors.fill: artistCover
                                    onClicked: artistPage.openartistData(artist)
                                }
                            }
                        }
                    }
                }
            }

            function turnToLine(lineId) {
                artistLineList.currentIndex = lineId
            }
        }

        Item{
            id: artistDataShow
            visible: false

            TikoButtonIcon{
                y: -10
                icon.source: "qrc:/image/back.png"
                onClicked: openartistLineList()
            }

            // 专辑信息背景
            Rectangle {
                id: artistDataBack
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 20
                width: parent.width - 60
                height: 270
                color: TikoSeit.backdropColor
                radius: 15
            }

            Image {
                id: artistDataCover
                width: 250
                height: 250
                sourceSize.width: width
                sourceSize.height: height
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 30
            }

            TikoTextLine {
                id: artistText
                width: parent.width - artistDataCover.width - 60
                anchors.left: artistDataCover.right
                anchors.leftMargin: 30
                anchors.top: artistDataCover.top
                exSize: 20
            }

            TikoTextLine {
                id: artistHelp
                width: parent.width - artistDataCover.width
                anchors.top: artistText.bottom
                anchors.topMargin: 10
                anchors.left: artistText.left
                exSize: 5
                opacity: 0.5
            }

            // 音乐列表
            ListView {
                id: musicList
                anchors.top: artistDataCover.bottom
                anchors.left: parent.left
                anchors.margins: 30
                width: parent.width - 60
                height: parent.height - artistDataBack.height - 40
                clip: true

                model: ListModel {
                    id: artistMusicList
                }

                delegate: CoreMusicLine {
                    width: musicList.width
                    listId: musicListId
                    music: musicCore
                }
            }
        }
    }

    function openartistLineList () {
        artistListShow.replace(artistLineShow)
        build()
    }

    function openartistData (artist) {
        artistListShow.replace(artistDataShow)

        var allTime = 0
        artistDataCover.source = "image://cover/file:" +  artist.musicList[0].coreId.toString()
        artistMusicList.clear()
        for (var i=0; i<artist.musicList.length; i++) {
            artistMusicList.append({
                                      musicListId: i,
                                      musicCore: artist.musicList[i]
                                  })
            allTime = artist.musicList[i].endTime
        }
        artistText.text = artist.name
        artistHelp.text = artist.musicList.length.toString()+" "+qsTr("首歌曲") +"-"+
                Base.timeToString(allTime)+" "+qsTr("歌曲长度")
    }

    function build () {
        artistModel.clear()
        var list = Core.artistLineList
        var all = 0
        var artistDataList = []
        for (var i=0; i<list.length; i++) {
            artistDataList = []
            for (var j=0; j<list[i].length; j++) {
                var musicId = list[i][j].musicList[0].coreId
                artistDataList.push({
                                       lineId: i,
                                       listId: j,
                                       artist: list[i][j],
                                       musicId: musicId
                                   })
            }

            artistModel.append({
                                  lineText: list[i][0].name[0],
                                  artistDataList: artistDataList,
                                  leng: list[i].length
                              })
            artistTurnButtonModel.append({
                                           lineText: list[i][0].name[0],
                                           lineId: i
                                       })
        }
    }
}
