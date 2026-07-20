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
        color: TikoSeit.theme.colorMaxBottom
        topLeftRadius: 10
        topRightRadius: 10
        z: -1
    }

    TikoButtonIcon {
        id: closeButton
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: TikoSeit.normalMargins
        icon.source: "qrc:/image/close.png"
        onClicked: CoreData.windowClose()
        level: 0
        //qsTr("关闭")
    }

    TikoButtonIcon{
        id: maxButton
        anchors.right: closeButton.left
        anchors.top: closeButton.top
        anchors.rightMargin: TikoSeit.normalMargins
        icon.source: CoreData.windows.visibility === 4 ? "qrc:/image/normal.png" : "qrc:/image/max.png"
        //qsTr("最大化")
        onClicked: CoreData.windowShowMax()
        level: 0
    }

    TikoButtonIcon{
        id: minButton
        anchors.top: closeButton.top
        anchors.right: maxButton.left
        anchors.rightMargin: TikoSeit.normalMargins
        icon.source: "qrc:/image/min.png"
        onClicked: CoreData.windowShowMin()
        level: 0
        //qsTr("最小化")
    }

    StackView {
        id: stackView
        anchors.top: closeButton.bottom
        width: parent.width
        height: parent.height - closeButton.height
        initialItem: seitPage
        clip: true
    }

    PageSeit {id: seitPage; visible: false}
    PageLibrary {id: libraryPage; visible: false}
    PageRecommend {id: recommendPage; visible: false}
    ShowPageAlbum {id: albumPlayer; visible: false}
    ShowPageArtist {id: artistPlayer; visible: false}
    ShowPagePlayList {id: playlistPlayer; visible: false }

    //切换到列表
    function turnToMusicList(page){
        playlistPlayer.setPlayListId(page)
        if(stackView.currentItem != playlistPlayer){
            stackView.replace(playlistPlayer)
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
        recommendPage.buildRand()
    }

    function turnToMain(){
        if(stackView.currentItem != recommendPage){
            stackView.replace(recommendPage)
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
