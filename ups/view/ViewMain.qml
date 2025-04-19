import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import Ups

Item {
    id: mainView
    clip: true

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

    PageAlbum {
        id: albumPage
        visible: false
    }

    PageArtist {
        id: artistPage
        visible: false
    }

    PageRecommend {
        id: mainPage
        visible: false
    }

    PlayerAlbum {
        id: albumPlayer
        visible: false
    }

    PlayerArtist {
        id: artistPlayer
        visible: false
    }

    PlayerTable {
        id: tablePlayer
        visible: false
    }

    //切换到列表
    function stackTable(page){
        tablePlayer.tableId = page
        if(stackView.currentItem !== tablePlayer){
            stackView.replace(tablePlayer)
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

    function turnToAlbum(){
        if(stackView.currentItem != albumPage){
            stackView.replace(albumPage)
        }
    }

    function turnToAlbumPlayer(album){
        albumPlayer.openAlbumData(album)
        if(stackView.currentItem != albumPlayer){
            stackView.replace(albumPlayer)
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
        // artistPage.build()
        // albumPage.build()
        mainPage.buildRand()
    }

    function turnToMain(){
        if(stackView.currentItem != mainPage){
            stackView.replace(mainPage)
        }
    }

    Component.onCompleted: {
        CoreData.mainTurnSeit.connect(turnToSeit)
        CoreData.mainTurnMain.connect(turnToMain)
        CoreData.mainTurnAlbumPage.connect(turnToAlbum)
        CoreData.mainTurnAlbumPlayer.connect(turnToAlbumPlayer)
        CoreData.mainTurnArtistPage.connect(turnToArtist)
        CoreData.mainTurnArtistPlayer.connect(turnToArtistPlayer)
    }
}
