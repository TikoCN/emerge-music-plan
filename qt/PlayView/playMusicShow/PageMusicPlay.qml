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
        height: pageMain.height - back.height
        y: back.height

        Component {
            id: playStyleCom1
            PlayStyle1 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
            }
        }

        Component {
            id: playStyleCom2
            PlayStyle2 {
                artist: pageMain.artist
                title: pageMain.title
                icon: pageMain.icon
            }
        }

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                const json = DataActive.getMusicJson(MediaPlayer.playingMusicId);
                artist = json.artist
                title = json.title
                icon = "image://cover/musicOnLine?id=" +
                        MediaPlayer.playingMusicId.toString() +
                        "&radius=10&highLight"
            }
        }
    }

    // 左侧栏目
    Drawer {
        id: typeSelect
        edge: Qt.RightEdge
        width: pageMain.width * 0.3
        height: pageMain.height

        Rectangle {
            id: background
            anchors.fill: parent
            color: TikoSeit.theme.baseTheme.backgroundNormal
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
        TikoButtonIconLittle {
            id: close
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("关闭")
            icon.source: "qrc:/image/close.png"
            onClicked: CoreData.windowClose()
        }

        //最大化
        TikoButtonIconLittle {
            id: max
            anchors.right: close.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("最大化")
            icon.source: "qrc:/image/max.png"
            onClicked: CoreData.windowShowMax()
        }

        //最小化
        TikoButtonIconLittle {
            id: min
            anchors.right: max.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("最小化")
            icon.source: "qrc:/image/min.png"
            onClicked: CoreData.windowShowMin()
        }

        //返回主页
        TikoButtonIconLittle {
            id: back
            anchors.left: parent.left
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("返回")
            icon.source: "qrc:/image/back.png"
            onClicked: window.stackCenter()
        }

        //样式
        TikoButtonIconLittle {
            id: style
            anchors.left: back.right
            anchors.margins: TikoSeit.emphasizeMargins
            //text: qsTr("样式")
            icon.source: "qrc:/image/style.png"
            onClicked: typeSelect.open()
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
