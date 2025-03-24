import QtQuick
import QtQuick.Controls
import Tiko
import TikoAPI
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
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        clip: true

        model: ListModel{
            id: alumbModel
        }

        delegate: Item {
            width: alumbLineList.width
            height: alumbLineText.height + alumbShow.height

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
                columns: parent.width / (150 + 20)
                spacing: 20
                height: (leng / columns + 1) * ((150 + 20))
                anchors.top: alumbLineText.bottom
                anchors.margins: 10

                Repeater{
                    model: ListModel {
                        id: alumbListModel
                    }

                    delegate: CoreAlumbButton {
                        alumb: alumbData
                    }
                }
            }
        }
    }


    function openAlumbLineList () {
        alumbListShow.replace(alumbLineShow)
        build()
    }

    function build () {
        alumbModel.clear()
        var list = Core.alumbLineList
        var all = 0
        var alumbDataList = []
        for (var i=0; i<list.length; i++) {
            alumbDataList = []
            for (var j=0; j<list[i].length; j++) {
                var musicId = list[i][j].musicList[0].coreId
                alumbDataList.push({
                                       lineId: i,
                                       listId: j,
                                       alumbData: list[i][j],
                                       musicId: musicId
                                   })
            }

            alumbModel.append({
                                  lineText: list[i][0].name[0],
                                  alumbDataList: alumbDataList,
                                  leng: list[i].length
                              })
            alumbTurnButtonModel.append({
                                            lineText: list[i][0].name[0],
                                            lineId: i
                                        })
        }
    }
}

