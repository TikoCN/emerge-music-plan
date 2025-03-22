import QtQuick
import QtQuick.Controls
import Tiko
import TikoAPI
import Widget


Item {
    id: alumbPage

    Item{
        id: alumbList
        visible: false
        ListView {
            id: alumbLineList
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 30
            width: parent.width - 60
            height: parent.height - 60
            ScrollBar.vertical: TikoBar{}

            model: ListModel{
                id: alumbModel
            }

            delegate: Item {
                width: alumbList.width
                height: alumbLineText.height + alumbShow.height

                TikoTextLine {
                    id: alumbLineText
                    text: lineText
                    width: parent.width
                    exSize: 10
                    color: TikoSeit.themeColor
                    opacity: 0.9
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
                        model: alumbDataList

                        delegate: Item{
                            width: 150
                            height: alumbCover.height + textLine.height + 10

                            Image {
                                id: alumbCover
                                width: parent.width
                                height: 150
                                source: "image://cover/file:" +  musicId.toString()
                                sourceSize.height: height
                                sourceSize.width: width
                                cache: false
                                asynchronous: true
                            }

                            TikoTextLine {
                                id: textLine
                                anchors.top: alumbCover.bottom
                                anchors.topMargin: 10
                                width: parent.width
                                text: alumb.name
                            }

                            MouseArea {
                                anchors.fill: alumbCover
                                onClicked: alumbPage.openAlumbData(alumb)
                            }
                        }
                    }
                }
            }
        }
    }

    Item{
        id: alumbDataShow
        visible: false
        TikoButtonIcon{
            y: -10
            icon.source: "qrc:/image/back.png"
            onClicked: alumbListShow.replace(alumbList)
        }

        // 专辑信息背景
        Rectangle {
            id: alumbDataBack
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
            width: parent.width - 60
            height: 270
            color: TikoSeit.backdropColor
            radius: 15
        }

        Image {
            id: alumbDataCover
            width: 250
            height: 250
            sourceSize.width: width
            sourceSize.height: height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 30
        }

        TikoTextLine {
            id: alumbText
            width: parent.width - alumbDataCover.width - 60
            anchors.left: alumbDataCover.right
            anchors.leftMargin: 30
            anchors.top: alumbDataCover.top
            exSize: 20
        }

        TikoTextLine {
            id: alumbHelp
            width: parent.width - alumbDataCover.width
            anchors.top: alumbText.bottom
            anchors.topMargin: 10
            anchors.left: alumbText.left
            exSize: 5
            opacity: 0.5
        }

        // 音乐列表
        ListView {
            id: musicList
            anchors.top: alumbDataCover.bottom
            anchors.left: parent.left
            anchors.margins: 30
            width: parent.width - 60
            height: parent.height - alumbDataBack.height - 40

            model: ListModel {
                id: alumbMusicList
            }

            delegate: CoreMusicLine {
                width: musicList.width
                listId: musicListId
                music: musicCore
            }
        }
    }

    StackView {
        id: alumbListShow
        initialItem: alumbList
        anchors.fill: parent
    }

    function openAlumbData (alumb) {
        alumbListShow.replace(alumbDataShow)

        var allTime = 0
        alumbDataCover.source = "image://cover/file:" +  alumb.musicList[0].coreId.toString()
        for (var i=0; i<alumb.musicList.length; i++) {
            alumbMusicList.append({
                                      musicListId: i,
                                      musicCore: alumb.musicList[i]
                                  })
            allTime = alumb.musicList[i].endTime
        }
        alumbText.text = alumb.name
        alumbHelp.text = alumb.musicList.length.toString()+" "+qsTr("首歌曲") +"-"+ allTime.toString()+" "+qsTr("歌曲长度")
    }

    function build () {
        alumbModel.clear();
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
                                       alumb: list[i][j],
                                       musicId: musicId
                                   })
            }

            alumbModel.append({
                                  lineText: list[i][0].name[0],
                                  alumbDataList: alumbDataList,
                                  leng: list[i].length
                              })
        }
    }
}
