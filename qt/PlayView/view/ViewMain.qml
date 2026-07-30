import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: mainView
    clip: true

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
        clip: true
    }

    property PageSeit pageSeit: null
    property PageLibrary pageLib: null
    property PageRecommend pageRecom: null
    property PageAlbum pageAlbum: null
    property PageArtist pageArtist: null
    property PagePlaylist pagePlaylist: null
    property PageMusicPlay pageDetail: null

    Component{id: comSeit;  PageSeit {visible: false}}
    Component{id: comLib; PageLibrary {visible: false}}
    Component{id: comRecom; PageRecommend {visible: false}}
    Component{id: comAlbum; PageAlbum {visible: false}}
    Component{id: comArtist; PageArtist {visible: false}}
    Component{id: comPlaylist; PagePlaylist {visible: false}}
    Component{id: comDetail; PageMusicPlay {visible: false; tspBg: true}}

    //底部导航
    ViewBottomBar{
        id: bottomView
        height: 90
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: TikoSeit.emphasizeMargins
    }

    function stackMode(mode) {
        switch (mode) {
        case 0:
            if (bottomView.visible !== true)
                bottomView.visible = true

            if (pageDetail !== null)
            pageDetail.setActive(false)
            return
        case 1:
            if (bottomView.visible !== false)
                bottomView.visible = false
            return
        }
    }

    //切换到列表
    function stackPlaylist(page){
        if (pagePlaylist === null) {
            if (comPlaylist.status === Component.Ready) {
                pagePlaylist = comPlaylist.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        pagePlaylist.setPlaylistId(page)
        if(stackView.currentItem != pagePlaylist){
            stackView.replace(pagePlaylist)
        }
    }

    //切换到设置
    function stackSeit(){
        if (pageSeit === null) {
            if (comSeit.status === Component.Ready) {
                pageSeit = comSeit.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        if(stackView.currentItem != pageSeit){
            stackView.replace(pageSeit)
        }
    }

    function stackArtist(artistId){
        if (pageArtist === null) {
            if (comArtist.status === Component.Ready) {
                pageArtist = comArtist.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        pageArtist.setArtistId(artistId)
        if(stackView.currentItem != pageArtist){
            stackView.replace(pageArtist)
        }
    }

    function stackLibrary(){
        if (pageLib === null) {
            if (comLib.status === Component.Ready) {
                pageLib = comLib.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        if(stackView.currentItem != pageLib){
            stackView.replace(pageLib)
        }
    }

    function stackAlbum(albumId){
        if (pageAlbum === null) {
            if (comAlbum.status === Component.Ready) {
                pageAlbum = comAlbum.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        pageAlbum.setAlbumId(albumId)
        if(stackView.currentItem != pageAlbum){
            stackView.replace(pageAlbum)
        }
    }

    function stackRcommend(){
        if (pageRecom === null) {
            if (comRecom.status === Component.Ready) {
                pageRecom = comRecom.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(0)
        if(stackView.currentItem != pageRecom){
            stackView.replace(pageRecom)
        }
    }

    function stackDetail(){
        if (pageDetail === null) {
            if (comDetail.status === Component.Ready) {
                pageDetail = comDetail.createObject(mainView)
            } else {
                return
            }
        }

        stackMode(1)
        pageDetail.setActive(true)
        if(stackView.currentItem != pageDetail){
            stackView.replace(pageDetail)
        }
    }

    Component.onCompleted: {
        CoreData.stackSeit.connect(stackSeit)
        CoreData.stackRcommend.connect(stackRcommend)
        CoreData.stackLibrary.connect(stackLibrary)
        CoreData.stackAlbum.connect(stackAlbum)
        CoreData.stackArtist.connect(stackArtist)
        CoreData.stackPlaylist.connect(stackPlaylist)
        CoreData.stackDetail.connect(stackDetail)
    }
}
