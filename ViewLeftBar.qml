import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MyAPI

Item {
    id: root
    clip: true

    //左侧导航
    ScrollView{
        id: leftView
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
            MyBarButton{
                width: parent.width
                id: mainPageButton
                text: qsTr("切换到主页")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: window.stackMain()
                icon.source: "qrc:/image/main.png"
            }

            //切换到设置
            MyBarButton{
                width: parent.width
                id: seitPageButton
                text: qsTr("切换到设置")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: mainView.turnToSeit();
                icon.source: "qrc:/image/seit.png"
            }
            MyBarButton{
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

                delegate: Component{
                    id: dirTableDelegate
                    MyBarButton{
                        width: parent.width
                        text: MediaPlayer.tableList[i].name
                        onClicked: mainView.stackTable(i)
                        icon.source: window.tableList[i].showCover
                        icon.width: 30
                        icon.height: 30
                    }
                }

                model: ListModel{
                    id: dirTableModel
                }
            }

            //新建列表
            MyBarButton{
                width: parent.width
                id: addTableButton
                text: qsTr("新建列表")
                onClicked: inputName.open()

                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: "qrc:/image/new.png"

                PopupInput{
                    id: inputName
                    text: qsTr("请输入列表名")
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

                delegate: Component{
                    id: userTableDelegate
                    MyBarButton{
                        width:  parent.width
                        text:  MediaPlayer.tableList[i].name
                        onClicked:  mainView.stackTable(i)
                        icon.source:  window.tableList[i].showCover
                        icon.width: 30
                        icon.height: 30
                    }
                }

                model: ListModel{
                    id: userTableModel
                }
            }
        }
    }

    //新建本地列表
    function addNewDirTable(table){
        if(mainView.addPlayTablePage(table)){
            dirTableModel.append({i: window.tableList.length-1})
        }
    }

    //新建用户列表
    function addNewUserTable(table){
        if(mainView.addPlayTablePage(table)){
            userTableModel.append({i: window.tableList.length-1})
        }
    }

    //关联
    Connections{
        target: MediaPlayer

        function onCppAddDirTable(tableId){
            root.addNewDirTable(tableId)
        }

        function onCppAddUserTable(tableId){
            root.addNewUserTable(tableId)
        }
    }
}
