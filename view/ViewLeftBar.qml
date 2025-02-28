import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import TikoAPI
import Tiko

Item {
    id: root
    clip: true
    property string select: qsTr("切换到设置")

    //左侧导航
    ScrollView{
        anchors.fill: parent
        ScrollBar.vertical.visible: false
        ScrollBar.horizontal.visible: false

        background: Rectangle{
            width: parent.width
            height: parent.height
            color: Setting.transparentColor//背景颜色
            opacity: 0.01
        }

        Column{
            spacing: 5
            width: parent.width
            height: parent.height

            //切换到主页
            TikoButton{
                width: parent.width
                id: mainPageButton
                text: qsTr("切换到主页")
                icon.source: "qrc:/image/main.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: window.stackMain()
            }

            //切换到设置
            TikoButton{
                width: parent.width
                id: seitPageButton
                text: qsTr("切换到设置")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked:{
                    root.select = this.text
                    mainView.turnToSeit();
                }
                icon.source: "qrc:/image/seit.png"

                //按钮选中边框边框
                Rectangle{
                    visible: parent.text === root.select
                    anchors.fill: parent
                    color: Setting.themeColor
                    opacity: 0.3
                    radius: 10
                }
            }
            TikoButton{
                width: parent.width
                text: qsTr("本地列表")
                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: "qrc:/image/dir.png"
                onClicked: dirTableListView.visible = !dirTableListView.visible
            }
            ListView{
                width: parent.width
                id: dirTableListView
                implicitHeight: childrenRect.height
                spacing: 5

                delegate: TikoButton{
                    width: dirTableListView.width
                    text: MediaPlayer.tableList[i].name
                    cache: false
                    icon.source: mainView.tableList[i].showCover
                    icon.width: 30
                    icon.height: 30
                    onClicked: {
                        root.select = this.text
                        mainView.stackTable(i)
                    }

                    //按钮选中边框边框
                    Rectangle{
                        visible: parent.text === root.select
                        anchors.fill: parent
                        color: Setting.themeColor
                        opacity: 0.3
                        radius: 10
                    }
                }


                model: ListModel{
                    id: dirTableModel
                }
            }

            //新建列表
            TikoButton{
                width: parent.width
                id: addTableButton
                text: qsTr("新建列表")
                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: "qrc:/image/new.png"
                onClicked: inputName.open()

                TikoPopupInput{
                    id: inputName
                    x: window.width / 2 - this.width / 2
                    y: window.height / 2 - this.height
                    text: qsTr("新建列表，请输入列表名")
                    onAccept: ()=>{
                                MediaPlayer.addTable(inputText)
                              }
                }
            }
            ListView{
                width: parent.width
                id: userTableListView
                implicitHeight: childrenRect.height
                spacing: 5

                delegate: TikoButton{
                    width: userTableListView.width
                    text: MediaPlayer.tableList[i].name
                    icon.source:  mainView.tableList[i].showCover
                    icon.width: 30
                    icon.height: 30
                    cache: false
                    onClicked: {
                        root.select = this.text
                        mainView.stackTable(i)
                    }

                    //按钮选中边框边框
                    Rectangle{
                        visible: parent.text === root.select
                        anchors.fill: parent
                        color: Setting.themeColor
                        opacity: 0.3
                        radius: 10
                    }
                }


                model: ListModel{
                    id: userTableModel
                }
            }
        }
    }

    function addDirTable(tableId){
        dirTableModel.append({i: tableId})
    }

    function addUserTable(tableId){
        userTableModel.append({i: tableId})
    }

    function clearData(){
        dirTableModel.clear()
        userTableModel.clear()
    }
}
