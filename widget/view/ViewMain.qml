import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import TikoAPI
import Tiko
import Widget

Item {
    id: mainView
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

    TikoButtonIcon{
        id: closeButton
        anchors.right: parent.right
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
        text: qsTr("关闭")
    }

    TikoButtonIcon{
        id: maxButton
        anchors.right: closeButton.left
        icon.source: window.visibility === 4 ? "qrc:/image/normal.png" : "qrc:/image/max.png"
        text: qsTr("最大化")
        onClicked: window.visibility === 4 ? window.showNormal() : window.showMaximized()
    }

    TikoButtonIcon{
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

    PageSeit {
        id: seitPage
        visible: false
    }

    PageAlumb {
        id: alumbPage
        visible: false
    }

    PageArtist {
        id: artistPage
        visible: false
    }

    PageMain {
        id: mainPage
        visible: false
    }

    PlayerAlumb {
        id: alumbPlayer
        visible: false
    }

    PlayerArtist {
        id: artistPlayer
        visible: false
    }

    //切换到列表
    function stackTable(page){
        if(stackView.currentItem !== mainView.tableList[page]){
            stackView.replace(mainView.tableList[page])
        }
    }

    //切换到设置
    function turnToSeit(){
        if(stackView.currentItem != seitPage){
            stackView.replace(seitPage)
        }
    }

    function turnToArtist(){
        if(stackView.currentItem != artistPage){
            stackView.replace(artistPage)
        }
    }

    function turnToArtistPlayer(artist){
        artistPlayer.openArtistData(artist)
        if(stackView.currentItem != artistPlayer){
            stackView.replace(artistPlayer)
        }
    }

    function turnToAlumb(){
        if(stackView.currentItem != alumbPage){
            stackView.replace(alumbPage)
        }
    }

    function turnToAlumbPlayer(alumb){
        alumbPlayer.openAlumbData(alumb)
        if(stackView.currentItem != alumbPlayer){
            stackView.replace(alumbPlayer)
        }
    }

    function addPlayTablePage(table){
        var component = Qt.createComponent("../player/PlayerTable.qml")

        if (component.status === Component.Ready) {
            var playTable = component.createObject(stackView, {tableId: table, visible: false})

            //插入链表
            mainView.tableList.push(playTable)
        }
    }

    function clearData(){
        //清空数据避免显示异常
        for(let i = 0; i < mainView.tableList.length; i++){
            mainView.tableList[i].destroy()
        }
        mainView.tableList = []

        turnToSeit()
    }

    function buildData(){
        artistPage.build()
        alumbPage.build()
        mainPage.buildRand()
    }

    function turnToMain(){
        if(stackView.currentItem != mainPage){
            stackView.replace(mainPage)
        }
    }
}
