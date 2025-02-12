import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MyAPI

Item {
    id: conterView
    clip: true

    //背景
    Rectangle{
        width: parent.width
        height: parent.height
        color: Setting.transparentColor//背景颜色
        opacity: 0.03
    }

    MyUiButton{
        id: closeButton
        anchors.right: parent.right
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
        text: qsTr("关闭")
    }

    MyUiButton{
        id: maxButton
        anchors.right: closeButton.left
        icon.source: window.visibility === 4 ? "qrc:/image/normal.png" : "qrc:/image/max.png"
        text: qsTr("最大化")
        onClicked: window.visibility === 4 ? window.showNormal() : window.showMaximized()
    }

    MyUiButton{
        id: minButton
        anchors.right: maxButton.left
        icon.source: "qrc:/image/min.png"
        onClicked: window.showMinimized()
        text: qsTr("最小化")
    }

    StackView{

        PageSeit{
            id: seitPage
            visible: false
        }

        id: stackView
        anchors.top: closeButton.bottom
        width: parent.width
        height: parent.height - closeButton.height

        initialItem: seitPage
    }

    //切换到列表
    function stackTable(page){
        if(stackView.currentItem !== window.tableList[page]){
            stackView.replace(window.tableList[page])
        }
    }

    //切换到设置
    function turnToSeit(){
        if(stackView.currentItem != seitPage){
            stackView.replace(seitPage)
        }
    }

    function addPlayTablePage(table){
        var component = Qt.createComponent("PlayerTable.qml")
        if (component.status === Component.Ready) {
            var playTable = component.createObject(stackView, {tableId: table})
            stackView.replace(playTable)

            //插入链表
            window.tableList.push(playTable)
            return true
        }
        else {
            return false
        }
    }
}
