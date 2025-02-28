import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import TikoAPI
import Tiko

Item {
    id: conterView
    clip: true

    property var tableList: []//列表指针
    property PageSeit seitPage: PageSeit{visible: false}

    //背景
    Rectangle{
        width: parent.width
        height: parent.height
        color: Setting.transparentColor//背景颜色
        opacity: 0.03
    }

    TikoUiButton{
        id: closeButton
        anchors.right: parent.right
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
        text: qsTr("关闭")
    }

    TikoUiButton{
        id: maxButton
        anchors.right: closeButton.left
        icon.source: window.visibility === 4 ? "qrc:/image/normal.png" : "qrc:/image/max.png"
        text: qsTr("最大化")
        onClicked: window.visibility === 4 ? window.showNormal() : window.showMaximized()
    }

    TikoUiButton{
        id: minButton
        anchors.right: maxButton.left
        icon.source: "qrc:/image/min.png"
        onClicked: window.showMinimized()
        text: qsTr("最小化")
    }

    StackView{
        id: stackView
        anchors.top: closeButton.bottom
        width: parent.width
        height: parent.height - closeButton.height
        initialItem: seitPage
    }

    //切换到列表
    function stackTable(page){
        if(stackView.currentItem !== conterView.tableList[page]){
            stackView.replace(conterView.tableList[page])
        }
    }

    //切换到设置
    function turnToSeit(){
        if(stackView.currentItem != seitPage){
            stackView.replace(seitPage)
        }
    }

    function addPlayTablePage(table){
        var component = Qt.createComponent("../core/PlayerTable.qml")

        if (component.status === Component.Ready) {
            var playTable = component.createObject(stackView, {tableId: table, visible: false})

            //插入链表
            conterView.tableList.push(playTable)
        }
    }

    function clearData(){
        //清空数据避免显示异常
        for(let i = 0; i < conterView.tableList.length; i++){
            conterView.tableList[i].destroy()
        }
        conterView.tableList = []

        turnToSeit()
    }
}
