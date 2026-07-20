import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView
MouseArea {
    id: pageMain
    property bool show: false
    property int type: 1
    property string icon: ""
    property string artist: ""
    property string title: ""
    hoverEnabled: true
    acceptedButtons: Qt.NoButton
    onContainsMouseChanged: heardItem.setState(containsMouse)

    onTypeChanged: updateShowModel()
    onShowChanged: updateShowModel()

    Loader{
        id: loaderStyle
        width: pageMain.width
        height: pageMain.height

        Component {
            id: playStyleCom1
            PlayStyle1 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
                show: pageMain.show
            }
        }

        Component {
            id: playStyleCom2
            PlayStyle2 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
                show: pageMain.show
            }
        }

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                const json = DataActive.getMusicJson(MediaPlayer.playingMusicId);
                artist = json.artist
                title = json.title
                icon = "image://cover/musicOnLine?id=" +
                        MediaPlayer.playingMusicId.toString() + "&radius=10"
            }
        }
    }

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
                icon.source: "qrc:/image/album.png"
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

    Item {
        id: heardItem
        width: parent.width
        height: childrenRect.height
        y: TikoSeit.emphasizeMargins

        //关闭
        TikoButtonIcon {
            id: close
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("关闭")
            icon.source: "qrc:/image/close.png"
            onClicked: CoreData.windowClose()
            level: 0
        }

        //最大化
        TikoButtonIcon {
            id: max
            anchors.right: close.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("最大化")
            icon.source: "qrc:/image/max.png"
            onClicked: CoreData.windowShowMax()
            level: 0
        }

        //最小化
        TikoButtonIcon {
            id: min
            anchors.right: max.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("最小化")
            icon.source: "qrc:/image/min.png"
            onClicked: CoreData.windowShowMin()
            level: 0
        }

        //返回主页
        TikoButtonIcon {
            id: back
            anchors.left: parent.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("返回")
            icon.source: "qrc:/image/back.png"
            onClicked: window.stackCenter()
            level: 0
        }

        //样式
        TikoButtonIcon {
            id: style
            anchors.left: back.right
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("样式")
            icon.source: "qrc:/image/style.png"
            onClicked: typeSelect.open()
            level: 0
        }

        ParallelAnimation {
            id: disappear
            NumberAnimation { target: heardItem; property: "y"; to: -heardItem.height; duration: 800 }
            NumberAnimation { target: heardItem; property: "opacity"; to: 0; duration: 800 }
        }
        ParallelAnimation {
            id: restore
            NumberAnimation { target: heardItem; property: "y"; to: TikoSeit.emphasizeMargins; duration: 800 }
            NumberAnimation { target: heardItem; property: "opacity"; to: 1; duration: 800 }
        }

        function setState(flag) {
            disappear.stop()
            restore.stop()

            if (flag) {
                restore.start()
            } else {
                disappear.start()
            }
        }
    }

    function actionStart(){
        pageMain.show = true
    }

    function actionEnd() {
        pageMain.show = false
    }

    function updateShowModel() {
        if(!pageMain.show){
            loaderStyle.sourceComponent = null
            return
        }

        switch(pageMain.type){
        case 1:
            loaderStyle.sourceComponent = playStyleCom1
            break
        case 2:
            loaderStyle.sourceComponent = playStyleCom2
            break
        }
    }
}
