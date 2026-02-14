import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: mainView

    property var playList: []
    property var playListId: []

    // 主体内容背景
    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentNormal
        topLeftRadius: 10
        topRightRadius: 10
    }

    TikoButtonIcon{
        id: closeButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: TikoSeit.normalMargins
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
        //qsTr("关闭")
    }

    TikoButtonIcon{
        id: maxButton
        anchors.right: closeButton.left
        anchors.top: closeButton.top
        anchors.rightMargin: TikoSeit.normalMargins
        icon.source: window.visibility === 4 ? "qrc:/image/normal.png" : "qrc:/image/max.png"
        //qsTr("最大化")
        onClicked: window.visibility === 4 ? window.showNormal() : window.showMaximized()
    }

    TikoButtonIcon{
        id: minButton
        anchors.top: closeButton.top
        anchors.right: maxButton.left
        anchors.rightMargin: TikoSeit.normalMargins
        icon.source: "qrc:/image/min.png"
        onClicked: window.showMinimized()
        //qsTr("最小化")
    }

    StackView{
        id: stackView
        anchors.top: closeButton.bottom
        width: parent.width
        height: parent.height - closeButton.height
        initialItem: seitPage
        z: -1
    }

    PageSeit {
        id: seitPage
        visible: false
    }

    PageLibrary {
        id: libraryPage
        visible: false
    }

    PageRecommend {
        id: mainPage
        visible: false
    }

    ShowPageAlbum {
        id: albumPlayer
        visible: false
    }

    ShowPageArtist {
        id: artistPlayer
        visible: false
    }

    Component{
        id: playlistListCom
        ShowPagePlayList {
            visible: false
        }
    }

    //切换到列表
    function turnToMusicList(page){

        var list = null
        for (var i=0;i<playList.length;i++) {
            if (playList[i].playlistId === page)
                list = playList[i]
        }

        if (list === null) {
            if (playlistListCom.status === Component.Ready) {
                list = playlistListCom.createObject(mainView)
                list.setPlayListId(page)
            }

            if (playList.length >= 3) {
                var newList = []
                for (i=1;i<playList.length;i++) {
                    newList.push(playList[i])
                }
                playList = newList
                playList.push(list)
            }
        }

        if(stackView.currentItem !== list){
            stackView.replace(list)
        }
    }

    //切换到设置
    function turnToSeit(){
        if(stackView.currentItem != seitPage){
            stackView.replace(seitPage)
        }
    }

    function turnToArtistPlayer(artistId){
        artistPlayer.setArtistId(artistId)
        if(stackView.currentItem != artistPlayer){
            stackView.replace(artistPlayer)
        }
    }

    function turnToLibraryPage(){
        if(stackView.currentItem != libraryPage){
            stackView.replace(libraryPage)
        }
    }

    function turnToAlbumPlayer(albumId){
        albumPlayer.setAlbumId(albumId)
        if(stackView.currentItem != albumPlayer){
            stackView.replace(albumPlayer)
        }
    }

    function buildData(){
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
        CoreData.mainTurnLibraryPage.connect(turnToLibraryPage)
        CoreData.mainTurnAlbumPlayer.connect(turnToAlbumPlayer)
        CoreData.mainTurnArtistPlayer.connect(turnToArtistPlayer)
        CoreData.mainTurnMusicList.connect(turnToMusicList)
    }
}
