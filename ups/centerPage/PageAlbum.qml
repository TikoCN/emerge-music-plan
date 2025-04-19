import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import Ups
Item {
    id: albumPage

    // 跳转按钮列表
    ListView {
        id: albumTurnButtonList
        orientation: ListView.Horizontal
        currentIndex: albumLineList.currentIndex
        width: parent.width - 60
        anchors.left: parent.left
        anchors.margins: 30
        spacing: 10
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4

        model: ListModel {
            id: albumTurnButtonModel
        }

        delegate: TikoTextLine {
            text: lineText
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3
            exSize: ListView.isCurrentItem ? 5 : 3

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    albumLineList.currentIndex = lineId
                    albumLineList.positionViewAtIndex(lineId, ListView.SnapPosition)
                }
            }
        }
    }

    ListView {
        id: albumLineList
        anchors.left: parent.left
        anchors.top: albumTurnButtonList.bottom
        anchors.margins: 30
        width: parent.width - 60
        height: parent.height - albumTurnButtonList.height - 60
        ScrollBar.vertical: TikoBarV{}
        clip: true

        model: ListModel{
            id: albumModel
        }

        delegate: Item {
            id: albumLine
            width: albumLineList.width
            height: albumLineText.height + albumShow.height + 10
            property int lineId: albumLineId

            TikoTextLine {
                id: albumLineText
                text: lineText
                width: parent.width
                exSize: 10
                color: TikoSeit.themeColor
                opacity: parent.ListView.isCurrentItem ? 1 : 0.3
            }

            Grid {
                id: albumShow
                width: parent.width
                columns: width / 160
                columnSpacing: 9
                rowSpacing: 10
                anchors.top: albumLineText.bottom
                anchors.margins: 10
                flow: Grid.LeftToRight

                Repeater{
                    model: albumDataList

                    delegate: CoreAlbumButton {
                        id: albumButton
                        // albumId: albumData
                    }
                }
            }
        }
    }

    function build () {
        albumModel.clear()
        var list = Core.albumList
        var all = 0
        var albumDataList = []
        var lineKey = ""
        var length = 0
        var line = 0
        for (var i=0; i<list.length; i++) {

            if (lineKey !== list[i].lineKey) {
                if (albumDataList.length > 0){
                    albumModel.append({
                                          lineText: lineKey,
                                          albumDataList: albumDataList,
                                          leng: length,
                                          albumLineId: line
                                      })
                    albumTurnButtonModel.append({
                                                    lineText: lineKey,
                                                    lineId: line
                                                })
                    length = 0
                    line++
                }
                lineKey = list[i].lineKey
                albumDataList = []
            }

            albumDataList.push({
                                   lineId: i,
                                   albumData: list[i],
                                   musicId: list[i].musicList[0].coreId
                               })
            length++
        }
    }
}

