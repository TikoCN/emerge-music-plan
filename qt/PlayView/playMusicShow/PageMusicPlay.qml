import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Window
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView
MouseArea {
    id: pageMain
    property bool active: false
    property bool tspBg: false
    property int type: 1
    property int bgStyle: 1
    property string icon: ""
    property string artist: ""
    property string title: ""
    property bool toolBarVisible: false
    hoverEnabled: true
    acceptedButtons: Qt.NoButton

    onTypeChanged: updateShowModel()
    onActiveChanged: updateShowModel()
    onTspBgChanged: setBgStyle()

    Timer {
        id: toolBarTimer
        interval: 3000
        onTriggered: pageMain.toolBarVisible = false
    }

    HoverHandler {
        onPointChanged: {
            if (CoreData.windows !== null && CoreData.windows.active) {
                pageMain.toolBarVisible = true
                toolBarTimer.restart()
            }
        }
    }

    Connections {
        target: CoreData.windows

        function onActiveChanged() {
            if (!CoreData.windows.active) {
                toolBarTimer.stop()
                pageMain.toolBarVisible = false
            }
        }
    }

    Rectangle {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: TikoSeit.normalMargins
        anchors.leftMargin: TikoSeit.normalMargins
        anchors.rightMargin: TikoSeit.normalMargins
        height: 48
        color: "transparent"
        opacity: pageMain.toolBarVisible ? 1 : 0
        enabled: pageMain.toolBarVisible && CoreData.windows !== null && CoreData.windows.active
        z: 10

        Behavior on opacity {
            NumberAnimation {
                duration: 180
            }
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.LeftButton
            onPressed: {
                const ownerWindow = pageMain.Window.window
                if (ownerWindow !== null)
                    ownerWindow.startSystemMove()
            }
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: TikoSeit.normalMargins
            anchors.verticalCenter: parent.verticalCenter
            spacing: TikoSeit.normalMargins

            TikoButtonIcon {
                icon.source: "qrc:/image/back.svg"
                level: 0
                onClicked: {
                    pageMain.setActive(false)
                    CoreData.windows.showType = 0
                }
            }

            TikoButtonIcon {
                icon.source: "qrc:/image/style.svg"
                level: 0
                onClicked: typeSelect.open()
            }
        }

        Row {
            anchors.right: parent.right
            anchors.rightMargin: TikoSeit.normalMargins
            anchors.verticalCenter: parent.verticalCenter
            spacing: TikoSeit.normalMargins

            TikoButtonIcon {
                icon.source: "qrc:/image/min.svg"
                level: 0
                onClicked: CoreData.windowShowMin()
            }

            TikoButtonIcon {
                icon.source: CoreData.windows.visibility === 4 ?
                                 "qrc:/image/normal.svg" : "qrc:/image/max.svg"
                level: 0
                onClicked: CoreData.windowShowMax()
            }

            TikoButtonIcon {
                icon.source: "qrc:/image/close.svg"
                level: 0
                onClicked: CoreData.windowClose()
            }
        }
    }

    //背景
    Loader{
        id: loaderBg
        anchors.fill: parent
        sourceComponent: bgStyle1

        Component {
            id: bgStyle1
            BackImageMove {
                iconUrl: pageMain.icon
            }
        }

        Component {
            id: bgStyle2
            BackWater {

            }
        }
    }

    //详情
    Loader{
        id: loaderStyle
        anchors.fill: parent
        sourceComponent: playStyleCom1

        Component {
            id: playStyleCom1
            PlayStyle1 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
                show: pageMain.active
                onSetBgType: {}
                onSetDetailType: {typeSelect.open()}
            }
        }

        Component {
            id: playStyleCom2
            PlayStyle2 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
                show: pageMain.active
                onSetBgType: {}
                onSetDetailType: {typeSelect.open()}
            }
        }
    }


    Connections{
        target: MediaPlayer
        function onPlayingMusicIdChanged(){
            loadCurrentMusic()
        }
    }

    Component.onCompleted: loadCurrentMusic()

    // 左侧栏目
    Drawer {
        id: typeSelect
        edge: Qt.RightEdge
        width: CoreData.windows.width * 0.3
        height: CoreData.windows.height

        Rectangle {
            id: background
            anchors.fill: parent
            color: TikoSeit.theme.colorBgDefault
        }

        GridView {
            id: gridView
            anchors.top: background.top
            anchors.bottom: background.bottom
            anchors.right: background.right
            anchors.left: background.left
            anchors.margins: TikoSeit.emphasizeMargins
            cellWidth: width / 2
            cellHeight: cellWidth

            delegate: TikoButtonIcon {
                icon.source: "qrc:/image/album.svg"
                icon.width: width
                icon.height: height
                width: gridView.cellWidth - TikoSeit.emphasizeMargins
                height: gridView.cellHeight - TikoSeit.emphasizeMargins
                onClicked: {
                    type = modelData.id
                }
            }

            model: [{id:1},{id:2}]
        }
    }

    function setActive(flag){
        pageMain.active = flag
    }

    function updateShowModel() {
        switch(pageMain.type){
        case 1:
            loaderStyle.sourceComponent = playStyleCom1
            break
        case 2:
            loaderStyle.sourceComponent = playStyleCom2
            break
        }
    }

    function setBgStyle() {
        if (tspBg) {
            loaderBg.sourceComponent = null
            return
        }

        switch(pageMain.bgStyle){
        case 1:
            loaderBg.sourceComponent = playStyleCom1
            break
        case 2:
            loaderBg.sourceComponent = playStyleCom2
            break
        }
    }

    function loadCurrentMusic() {
        const musicId = MediaPlayer.playingMusicId
        if (musicId < 0) {
            artist = ""
            title = ""
            icon = ""
            return
        }

        const core = MusicLibrary.getData(musicId)
        artist = core.artist
        title = core.title
        icon = "image://cover/musicOnLine?id=" +
                musicId.toString() + "&radius=10"
    }
}
