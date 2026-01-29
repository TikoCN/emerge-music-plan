import QtQuick
import QtQuick.Layouts
import MediaerAPI
import Tiko
import DataType
import PlayView

//展示音乐行
Item {
    id: musicLine
    implicitHeight: 70
    implicitWidth: 100
    clip: true

    property int listId: 0
    property int musicId: -1
    property int musicLevel: -1
    property int duration: -1
    property int playNumber: -1
    property bool isLove: false
    property bool isLittle: false
    property string musicTitle: ""
    property string artist : ""
    property string album: ""
    property string lastEdit: ""
    signal playMusic()

    Component.onCompleted:{
        const json = DataActive.getMusicJson(musicId);
        musicTitle = json.title
        artist = json.artist
        album = json.album
        musicLevel = json.level
        duration = json.duration
        playNumber = json.playNumber
        isLove = json.isLove
        lastEdit = json.lastEdit
    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: (mouse) => {
                       switch(mouse.button){
                           case Qt.LeftButton:
                           playMusic()
                           break
                           case Qt.RightButton:
                           createMenu(musicLine)
                           break
                       }
                   }

        //主体
        Row{
            x:10
            y:10
            width: parent.width - 20
            height: parent.height - 20
            spacing: 6

            //封面
            AutoCoverImage {
                id: cover
                width: 50
                height: 50
                normalUrl: "qrc:/image/music.png"
                loadUrl: "image://cover/musicFile?id=" +
                         musicId.toString() +
                         "&radius=10"
            }

            Column{
                id: coreName
                width: isLittle ? (parent.width - cover.width - 30) / 2 :
                                  (parent.width - toolItem.width - cover.width - durationTextView.width - 30) / 2

                TikoTextLine{
                    text: musicTitle
                    exSize: 3
                    font.bold: true
                    height: 30
                    width: parent.width
                }
                TikoTextLine{
                    text: artist
                    height: 20
                    width: parent.width
                }
            }

            TikoTextLine{
                text: album
                width: coreName.width
                height: 50
            }

            // 使用 Loader 延迟加载
            Loader {
                id: toolItem
                width: 250
                height: 50

                // 只在鼠标悬停时加载组件
                active: mouseArea.containsMouse && !musicLine.isLittle

                sourceComponent: Component {
                    Row {
                        spacing: 10
                        anchors.fill: parent

                        TikoButtonIcon{
                            text: qsTr("喜爱")
                            width: 30
                            height: parent.height
                            onClicked: {
                                musicLine.isLove = !musicLine.isLove
                                DataActive.updateMusicLove(musicLine.musicId, musicLine.isLove)
                            }
                            icon.source: musicLine.isLove ?
                                             "qrc:/image/love.png" : "qrc:/image/unlove.png"
                            useAutoColor: !musicLine.isLove
                            cache: true
                        }

                        // 评级
                        Row{
                            width: 100
                            height: 30
                            spacing: 1

                            Repeater{
                                delegate: TikoButtonIcon{
                                    width: 20
                                    height: 50
                                    hover: 0
                                    cache: true
                                    onClicked: {
                                        // 设置评级逻辑
                                        musicLine.musicLevel = level
                                        DataActive.updateMusicLevel(musicLine.musicId, musicLine.musicLevel)
                                    }
                                    icon.source: musicLine.musicLevel >= level ?
                                                     "qrc:/image/int.png" : "qrc:/image/unInt.png"
                                    useAutoColor: !(musicLine.musicLevel >= level)
                                }
                                model: ListModel{
                                    ListElement{level: 1}
                                    ListElement{level: 2}
                                    ListElement{level: 3}
                                    ListElement{level: 4}
                                    ListElement{level: 5}
                                }
                            }
                        }
                        TikoButtonIcon{
                            text: qsTr("更多")
                            width: 30
                            height: parent.height
                            onClicked: createMenu(musicLine)
                            icon.source: "qrc:/image/else.png"
                            cache: true
                        }
                    }
                }
            }


            //时间参数
            TikoTextLine{
                id: durationTextView
                text: BaseTool.typeConversion.timeToString(duration)
                width: CoreData.fontW * 3
                height: 50
            }
        }
    }

    //底层背景
    Rectangle{
        id: background
        anchors.fill: parent
        radius: 5
        color: Setting.transparentColor//背景颜色
    }

    states: [
        State {
            name: "hovered"
            when: mouseArea.containsMouse
            PropertyChanges { target: background; opacity: 0.2 }
        },
        State {
            name: "normal"
            when: !mouseArea.containsMouse
            PropertyChanges {
                target: background;
                opacity: (musicLine.listId % 2 === 1) ? 0.1 : 0.05
            }
        }
    ]

    Component {
        id: menuMusicComponent
        MenuMusicPlayList {
            musicId: musicLine.musicId
            playlistId: playlistId
        }
    }

    Component {
        id: menuMusicPlayListComponent
        MenuMusicPlayList {
            musicId: musicLine.musicId
            playlistId: playlistId
        }
    }

    function createMenu(parent){
        if (menuMusicComponent.status === Component.Ready){
            let menu = menuMusicComponent.createObject(parent)
            menu.popup()
        }
        else
            console.log(menuComponent.errorString())
    }

}

