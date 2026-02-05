import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView

TikoRightVessel {
    Layout.preferredHeight: this.height
    titleButton.textLine.text: qsTr("更新数据")

    vessel: Item{
        height: childrenRect.height

        TikoButtonBorder{
            id: selectDirButton
            width: 150
            height: 50
            text: qsTr("选择音乐文件地址")
            onClicked: selectMusicDir.open()
        }

        TikoButtonBorder{
            id: reloadButton
            anchors.left: selectDirButton.right
            anchors.leftMargin: 10
            width: selectDirButton.width
            height: 50
            text: qsTr("加载音乐")
            onClicked: {
                CoreData.clearData()
                Setting.loadMusicCores()
            }
        }

        TikoButtonBorder{
            id: deleteOverData
            anchors.left: reloadButton.right
            anchors.leftMargin: 10
            width: reloadButton.width
            height: 50
            text: qsTr("清除过时数据")
            onClicked: {
                CoreData.clearData()
                DataActive.clearNullItem()
            }
        }

        Rectangle{
            color: Setting.transparentColor
            anchors.fill: sourceListColumn
            opacity: 0.05
        }

        Column{
            id: sourceListColumn
            anchors.top: selectDirButton.bottom
            anchors.topMargin: 10
            width: parent.width * 0.8

            Repeater{
                model: ListModel{
                    id: musicSourceModel
                }

                delegate: TikoTextLine {
                    text: url
                    background.opacity: {
                        if(i % 2 === 0){
                            return 0.05
                        }
                        else{
                            return 0
                        }
                    }
                    height: 40
                    width: sourceListColumn.width
                    padding: 10

                    TikoButtonIcon{
                        anchors.right: parent.right
                        anchors.verticalCenter: parent.verticalCenter
                        icon.source: "qrc:/image/close.png"
                        onClicked: {
                            Setting.removeUrl(url)
                            sourceListColumn.bulidSoure()
                        }
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
