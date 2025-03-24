import QtQuick
import QtQuick.Controls
import Tiko
import TikoAPI
import Widget

Item {
    id: artistPage

    // 跳转按钮列表
    ListView {
        id: artistTurnButtonList
        orientation: ListView.Horizontal
        currentIndex: artistLineList.currentIndex
        width: parent.width - 60
        anchors.left: parent.left
        anchors.margins: 30
        spacing: 10
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4

        model: ListModel {
            id: artistTurnButtonModel
        }

        delegate: TikoTextLine {
            text: lineText
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3
            exSize: ListView.isCurrentItem ? 5 : 3

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    artistLineList.currentIndex = lineId
                    artistLineList.positionViewAtIndex(lineId, ListView.SnapPosition)
                }
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
        ScrollBar.vertical: TikoBarV{}
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        clip: true

        model: ListModel{
            id: artistModel
        }

        delegate: Item {
            id: artistLine
            width: artistLineList.width
            height: artistLineText.height + artistShow.height + 10
            property int lineId: artistLineId

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
                columns: width / 160
                columnSpacing: 9
                rowSpacing: 10
                anchors.top: artistLineText.bottom
                anchors.margins: 10
                flow: Grid.LeftToRight

                Repeater{
                    model: artistDataList

                    delegate: CoreArtistButton {
                        id: artistButton
                        artist: artistData
                    }
                }
            }
        }
    }

    function openartistLineList () {
        artistListShow.replace(artistLineShow)
        build()
    }

    function build () {
        artistModel.clear()
        var list = Core.artistLineList
        var all = 0
        for (var i=0; i<list.length; i++) {
            var artistDataList = []
            for (var j=0; j<list[i].length; j++) {
                var musicId = list[i][j].musicList[0].coreId
                artistDataList.push({
                                       lineId: i,
                                       listId: j,
                                       artistData: list[i][j],
                                       musicId: musicId
                                   })
            }

            artistModel.append({
                                  lineText: list[i][0].name[0],
                                  artistDataList: artistDataList,
                                  leng: list[i].length,
                                  artistLineId: i
                              })
            artistTurnButtonModel.append({
                                            lineText: list[i][0].name[0],
                                            lineId: i
                                        })
        }
    }
}

