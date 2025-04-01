import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import Widget

Item {
    id: alumbPage

    // 跳转按钮列表
    ListView {
        id: alumbTurnButtonList
        orientation: ListView.Horizontal
        currentIndex: alumbLineList.currentIndex
        width: parent.width - 60
        anchors.left: parent.left
        anchors.margins: 30
        spacing: 10
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4

        model: ListModel {
            id: alumbTurnButtonModel
        }

        delegate: TikoTextLine {
            text: lineText
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3
            exSize: ListView.isCurrentItem ? 5 : 3

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    alumbLineList.currentIndex = lineId
                    alumbLineList.positionViewAtIndex(lineId, ListView.SnapPosition)
                }
            }
        }
    }

    ListView {
        id: alumbLineList
        anchors.left: parent.left
        anchors.top: alumbTurnButtonList.bottom
        anchors.margins: 30
        width: parent.width - 60
        height: parent.height - alumbTurnButtonList.height - 60
        ScrollBar.vertical: TikoBarV{}
        clip: true

        model: ListModel{
            id: alumbModel
        }

        delegate: Item {
            id: alumbLine
            width: alumbLineList.width
            height: alumbLineText.height + alumbShow.height + 10
            property int lineId: alumbLineId

            TikoTextLine {
                id: alumbLineText
                text: lineText
                width: parent.width
                exSize: 10
                color: TikoSeit.themeColor
                opacity: parent.ListView.isCurrentItem ? 1 : 0.3
            }

            Grid {
                id: alumbShow
                width: parent.width
                columns: width / 160
                columnSpacing: 9
                rowSpacing: 10
                anchors.top: alumbLineText.bottom
                anchors.margins: 10
                flow: Grid.LeftToRight

                Repeater{
                    model: alumbDataList

                    delegate: CoreAlumbButton {
                        id: alumbButton
                        alumb: alumbData
                    }
                }
            }
        }
    }

    function build () {
        alumbModel.clear()
        var list = Core.alumbList
        var all = 0
        var alumbDataList = []
        var lineKey = ""
        var length = 0
        var line = 0
        for (var i=0; i<list.length; i++) {

            if (lineKey !== list[i].lineKey) {
                if (alumbDataList.length > 0){
                    alumbModel.append({
                                          lineText: lineKey,
                                          alumbDataList: alumbDataList,
                                          leng: length,
                                          alumbLineId: line
                                      })
                    alumbTurnButtonModel.append({
                                                    lineText: lineKey,
                                                    lineId: line
                                                })
                    length = 0
                    line++
                }
                lineKey = list[i].lineKey
                alumbDataList = []
            }

            alumbDataList.push({
                                   lineId: i,
                                   alumbData: list[i],
                                   musicId: list[i].musicList[0].coreId
                               })
            length++
        }
    }
}

