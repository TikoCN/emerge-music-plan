import QtQuick
import Tiko
import MediaerAPI
import DataType
import PlayView

Item {
    id: musicButton
    width: CoreData.cellItemWidth
    height: CoreData.cellItemHeight

    property int musicId: -1
    property int type: 0
    property string musicTitle: ""
    property string artist: ""
    signal playMusic()

    Component.onCompleted: {
        const Json = DataActive.getMusicJson(musicId);
        musicTitle = Json.title
        artist = Json.artist
    }

    //背景
    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.backgroundNormal
        opacity: mouse.containsMouse ? 0.1 : 0
        radius: 10
    }

    MouseArea{
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: (mouse) => {
                       switch(mouse.button){
                           case Qt.LeftButton:
                           playMusic()
                           break
                           case Qt.RightButton:
                           createMenu(musicButton)
                           break
                       }
                   }

        //展示主体
        Item {
            id: showItem
            height: childrenRect.height
            x: CoreData.cellItemSpace
            y: CoreData.cellItemSpace
            width: CoreData.cellImageWidth

            AutoCoverImage {
                id: musicCover
                normalUrl: "qrc:/image/exe.png"
                loadUrl: "image://cover/musicFile?id=" +
                         musicId.toString() +
                         "&radius=10"
                extraLoadFlag: musicId !== -1
                width: CoreData.cellImageWidth
                height: CoreData.cellImageHeight
            }

            // 播放按钮
            TikoButtonIcon {
                width: 30
                height: width
                icon.source: "qrc:/image/play.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.bottom: musicCover.bottom
                anchors.left: musicCover.left
                anchors.margins: musicCover.width * 0.05
                visible: mouse.containsMouse
                onClicked: playMusic()
            }

            // 菜单按钮
            TikoButtonIcon {
                width: 30
                height: width
                icon.source: "qrc:/image/more.png"
                icon.width: width / 2
                icon.height: height / 2
                anchors.top: musicCover.top
                anchors.right: musicCover.right
                anchors.margins: musicCover.width * 0.05
                visible: mouse.containsMouse
                onClicked: createMenu(this)
            }
        }

        TikoTextLine {
            id: titleText
            text: musicTitle
            width: parent.width
            anchors.top: showItem.bottom
            anchors.left: showItem.left
            anchors.topMargin: CoreData.cellItemSpace
        }

        TikoTextLine {
            id: artistText
            text: artist
            width: parent.width
            opacity: 0.7
            anchors.top: titleText.bottom
            anchors.left: showItem.left
        }
    }

    Component {
        id: menuComponent
        MenuMusic {
            musicId: musicButton.musicId
        }
    }

    function createMenu(parent){
        if (menuComponent.status === Component.Ready){
            let menu = menuComponent.createObject(parent)
            menu.popup()
        }
        else
            console.log(menuComponent.errorString())
    }
}
