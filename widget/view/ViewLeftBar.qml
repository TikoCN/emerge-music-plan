import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import ControlAPI   
import Tiko

Item {
    id: root
    property string showText: ""

    //左侧导航
    ScrollView{
        ScrollBar.vertical.visible: false
        ScrollBar.horizontal.visible: false
        height: parent.height
        anchors.right: parent.right
        anchors.margins: 10
        width: parent.width - 10

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
            TikoButtonNormal{
                width: parent.width
                id: mainPageButton
                text: qsTr("主页")
                iconSource: "qrc:/image/main.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClickLeft: {
                    root.showText = text
                    mainView.turnToMain()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            TikoButtonNormal{
                id: artistButton
                width: parent.width
                text: qsTr("专辑")
                iconSource: "qrc:/image/alumb.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClick: {
                    root.showText = text
                    mainView.turnToAlumb()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            TikoButtonNormal{
                width: parent.width
                text: qsTr("作曲家")
                iconSource: "qrc:/image/artist.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClick: {
                    root.showText = text
                    mainView.turnToArtist()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            //切换到设置
            TikoButtonNormal{
                width: parent.width
                id: seitPageButton
                text: qsTr("设置")
                anchors.horizontalCenter: parent.horizontalCenter
                onClickLeft:{
                    root.showText = text
                    mainView.turnToSeit();
                }
                iconSource: "qrc:/image/seit.png"

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            //新建列表
            TikoButtonNormal{
                width: parent.width
                id: addTableButton
                text: qsTr("新建列表")
                anchors.horizontalCenter: parent.horizontalCenter
                iconSource: "qrc:/image/new.png"
                onClickLeft: inputName.open()

                TikoPopupInput{
                    id: inputName
                    text: qsTr("新建列表，请输入列表名")
                    onAccept: {
                        Core.appendTable(inputName.inputText)
                        inputName.setNormalText()
                    }
                }
            }

            // 插入的列表按钮
            ListView{
                width: parent.width
                id: userTableListView
                implicitHeight: childrenRect.height
                spacing: 5

                delegate: TikoButtonNormal{
                    width: userTableListView.width
                    text: Core.tableList[i].name
                    iconSource: mainView.tableList[i].showCover
                    iconWidth: 30
                    iconHeight: 30
                    cache: false
                    onClickLeft: {
                        root.showText = text
                        mainView.stackTable(i)
                    }
                    onClickRight: tableMenu.open()

                    Rectangle {
                        color: TikoSeit.transparentColor
                        opacity: 0.2
                        visible: root.showText === parent.text
                        anchors.fill: parent
                        radius: 15
                    }
                }

                model: ListModel{
                    id: userTableModel
                }
            }
        }
    }

    function addTable(tableId){
        userTableModel.append({i: tableId})
    }

    function clearData(){
        userTableModel.clear()
    }
}
