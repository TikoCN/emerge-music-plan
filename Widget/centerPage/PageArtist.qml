import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
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
        clip: true

        model: ListModel{
            id: artistModel
        }

        delegate: Item {
            id: artistLine
            width: artistLineList.width
            height: childrenRect.height
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

    function build () {
        artistModel.clear()
        var list = Core.artistList
        var all = 0
        var artistDataList = []
        var lineKey = ""
        var length = 0
        var line = 0
        for (var i=0; i<list.length; i++) {

            if (lineKey !== list[i].lineKey) {
                if (artistDataList.length > 0) {
                    artistModel.append({
                                          lineText: list[i].lineKey,
                                          artistDataList: artistDataList,
                                          leng: length,
                                          artistLineId: line
                                      })
                    artistTurnButtonModel.append({
                                                    lineText: list[i].lineKey,
                                                    lineId: line
                                                })
                    length = 0
                    line++
                }
                lineKey = list[i].lineKey
                artistDataList = []
            }

            artistDataList.push({
                                   lineId: i,
                                   artistData: list[i],
                                   musicId: list[i].musicList[0].coreId
                               })
            length++
        }
    }
}

