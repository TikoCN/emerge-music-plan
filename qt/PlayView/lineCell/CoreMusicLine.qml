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

    required property var model
    required property int index

    property int listId: index
    property int playlistId: -1
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
                           musicLine.playMusic(musicLine.model.id, musicLine.listId)
                           break
                           case Qt.RightButton:
                           createMenu(musicLine, mouse.x, mouse.y)
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
                normalUrl: "qrc:/image/music.svg"
                baseUrl: "image://cover/musicFile?id=" +
                         musicLine.model.id.toString() +
                         "&radius=10"
            }

            Column{
                id: coreName
                width: isLittle ? (parent.width - cover.width - 30) / 2 :
                                  (parent.width - toolItem.width - cover.width - durationTextView.width - 30) / 2

                TikoTextLine{
                    text: musicLine.model.title
                    height: 30
                    width: parent.width
                    level: 2
                }
                TikoTextLine{
                    text: musicLine.model.artist
                    height: 20
                    width: parent.width
                    level: 0
                }
            }

            TikoTextLine{
                text: musicLine.model.album
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
                            onClicked: MusicLibrary.updateLove(musicLine.model.id, !musicLine.model.isLove)
                            icon.source: musicLine.model.isLove ?
                                             "qrc:/image/love.svg" : "qrc:/image/unlove.svg"
                        }

                        Row{
                            width: 100
                            height: 30
                            spacing: 1

                            Repeater{
                                delegate: TikoButtonIcon{
                                    required property int index
                                    width: 20
                                    height: 50
                                    onClicked: MusicLibrary.updateLevel(musicLine.model.id, index + 1)
                                    icon.source: musicLine.model.level >= index + 1 ?
                                                     "qrc:/image/int.svg" : "qrc:/image/unInt.svg"
                                }
                                model: 5
                            }
                        }
                        TikoButtonIcon{
                            width: 30
                            height: parent.height
                            onClicked: mouse => createMenu(musicLine, mouse.x, mouse.y)
                            icon.source: "qrc:/image/more.svg"
                        }
                    }
                }
            }

            TikoTextLine{
                id: durationTextView
                text: BaseTool.typeConversion.timeToString(musicLine.model.duration)
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
        MenuMusicPlaylist {
            musicId: musicLine.model.id
            playlistId: musicLine.playlistId
        }
    }

    function createMenu(parent, x, y){
        if (menuMusicComponent.status === Component.Ready){
            let menu = menuMusicComponent.createObject(parent)
            menu.playlistChanged.connect(function() {
                const ownerList = musicLine.ListView.view
                if (ownerList !== null && typeof ownerList.reset === "function")
                    ownerList.reset()
            })
            menu.popupAt(parent, x, y)
        }
        else
            console.log(menuComponent.errorString())
    }
}
