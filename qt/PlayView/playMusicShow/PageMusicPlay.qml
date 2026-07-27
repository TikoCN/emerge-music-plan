import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView
MouseArea {
    id: pageMain
    property bool active: false
    property int type: 1
    property string icon: ""
    property string artist: ""
    property string title: ""
    hoverEnabled: true
    acceptedButtons: Qt.NoButton

    onTypeChanged: updateShowModel()
    onActiveChanged: updateShowModel()

    //背景
    Loader{
        id: loaderBg
        anchors: parent
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

        Connections{
            target: MediaPlayer.player
            function onSourceChanged(){
                const json = MusicLibrary.getJson(MediaPlayer.playingMusicId);
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

    function actionStart(flag){
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
}
