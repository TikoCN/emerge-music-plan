import QtQuick
import QtQuick.Layouts
import MediaerAPI
import Tiko
import DataType
import PlayView

Item {
    id: musicLine
    implicitHeight: 70
    implicitWidth: 100
    clip: true

    property int listId: 0
    property bool isLittle: false
    signal playMusic(int musicId, int listId)

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        onClicked: (mouse) => {
                       switch(mouse.button){
                           case Qt.LeftButton:
                           playMusic(model.id, listId)
                           break
                           case Qt.RightButton:
                           createMenu(musicLine)
                           break
                       }
                   }

        Row{
            x:10
            y:10
            width: parent.width - 20
            height: parent.height - 20
            spacing: 6

            AutoCoverImage {
                id: cover
                width: 50
                height: 50
                normalUrl: "qrc:/image/music.png"
                baseUrl: "image://cover/musicFile?id=" +
                         model.id.toString() +
                         "&radius=10"
            }

            Column{
                id: coreName
                width: isLittle ? (parent.width - cover.width - 30) / 2 :
                                  (parent.width - toolItem.width - cover.width - durationTextView.width - 30) / 2

                TikoTextLine{
                    text: model.title
                    height: 30
                    width: parent.width
                    level: 2
                }
                TikoTextLine{
                    text: model.artist
                    height: 20
                    width: parent.width
                    level: 0
                }
            }

            TikoTextLine{
                text: model.album
                width: coreName.width
                height: 50
            }

            Loader {
                id: toolItem
                width: 250
                height: 50

                active: mouseArea.containsMouse && !musicLine.isLittle

                sourceComponent: Component {
                    Row {
                        spacing: 10
                        anchors.fill: parent

                        TikoButtonIcon{
                            width: 30
                            height: parent.height
                            onClicked: MusicLibrary.updateLove(model.id, !model.isLove)
                            icon.source: model.isLove ?
                                             "qrc:/image/love.png" : "qrc:/image/unlove.png"
                        }

                        Row{
                            width: 100
                            height: 30
                            spacing: 1

                            Repeater{
                                delegate: TikoButtonIcon{
                                    width: 20
                                    height: 50
                                    onClicked: MusicLibrary.updateLevel(model.id, level)
                                    icon.source: model.level >= level ?
                                                     "qrc:/image/int.png" : "qrc:/image/unInt.png"
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
                            width: 30
                            height: parent.height
                            onClicked: createMenu(musicLine)
                            icon.source: "qrc:/image/more.png"
                        }
                    }
                }
            }

            TikoTextLine{
                id: durationTextView
                text: BaseTool.typeConversion.timeToString(model.duration)
                width: CoreData.fontW * 3
                height: 50
            }
        }
    }

    Rectangle{
        id: background
        anchors.fill: parent
        radius: 5
        color: TikoSeit.theme.colorBgDefault
        z: -1
    }

    states: [
        State {
            name: "hovered"
            when: mouseArea.containsMouse
            PropertyChanges { target: background; color: TikoSeit.theme.colorHighlight}
        },
        State {
            name: "normal"
            when: !mouseArea.containsMouse
            PropertyChanges {target: background; opacity: (musicLine.listId % 2 === 1) ? 1 : 0.5}
        }
    ]

    Component {
        id: menuMusicComponent
        MenuMusicPlayList {
            musicId: model.id
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