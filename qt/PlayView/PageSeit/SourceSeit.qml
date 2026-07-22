import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.preferredHeight: height
    titleButton.textLine.text: qsTr("资源设置")

    vessel: Item{
        width: bgLoader.width
        height: childrenRect.height

        TikoButtonDefault{
            id: selectDirButton
            textLine.text: qsTr("添加文件夹")
            onClicked: selectMusicDir.open()
        }

        TikoButtonDefault{
            id: reloadButton
            anchors.left: selectDirButton.right
            anchors.leftMargin: 10
            textLine.text: qsTr("加载音乐")
            onClicked: {
                CoreData.clearData()
                Setting.loadMusicCores()
            }
        }

        TikoButtonDefault{
            id: deleteOverData
            anchors.left: reloadButton.right
            anchors.leftMargin: 10
            textLine.text: qsTr("清除失效数据")
            onClicked: {
                PlayListLibrary.clearNullItem()
                MusicLibrary.startClearInvalidData()
            }
        }

        Rectangle{
            anchors.fill: sourceListColumn
            radius: 10
            color: TikoSeit.theme.colorBgView
        }

        ListView{
            id: sourceListColumn
            anchors.top: selectDirButton.bottom
            anchors.topMargin: 10
            width: parent.width
            height: 300

            model: ListModel{
                id: musicSourceModel
            }

            delegate: Item {
                width: sourceListColumn.width
                height: 50

                TikoTextLine {
                    id: sourceTextItem
                    text: url
                    anchors.left: parent.left
                    anchors.leftMargin: TikoSeit.subitemSpace
                    anchors.verticalCenter: parent.verticalCenter
                }

                TikoButtonIcon{
                    anchors.right: parent.right
                    anchors.rightMargin: TikoSeit.subitemSpace
                    anchors.verticalCenter: parent.verticalCenter
                    icon.source: "qrc:/image/close.png"
                    level: 0
                    onClicked: {
                        Setting.removeUrl(url)
                        sourceListColumn.bulidSoure()
                    }
                }
            }

            FolderDialog{
                id: selectMusicDir
                onAccepted: {
                    var url = selectMusicDir.selectedFolder
                    Setting.sourceList.push(url)
                    sourceListColumn.bulidSoure()
                }
            }

            //插入新条目
            function bulidSoure(){
                musicSourceModel.clear()
                for(var i=0; i<Setting.sourceList.length; i++){
                    musicSourceModel.append({url:Setting.sourceList[i], i:i})
                }
            }

            //初始化
            Component.onCompleted: bulidSoure()
        }
    }
}
