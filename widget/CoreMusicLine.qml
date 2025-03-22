import QtQuick
import QtQuick.Layouts
import TikoAPI
import Tiko

Item {
    id: musicLine
    implicitHeight: 70
    implicitWidth: 100
    clip: true

    property int tableId: 0
    property int listId: 0
    property int musicId: music.coreId
    property var music

    MouseArea{
        onClicked:(mouse)=>{
            if(mouse.button === Qt.RightButton){
                CoreData.openMenuMusic(musicLine)
            }
            else{
                musicLine.play()
            }
        }
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        anchors.fill:parent
    }

    Rectangle{
        anchors.fill: parent
        radius: 5
        color: Setting.transparentColor//背景颜色
        opacity:{
            if(musicLine.listId % 2 === 1){
                opacity = 0.1
            }
            else{
                opacity = 0.05
            }
        }
    }

    Row{
        x:10
        y:10
        width: parent.width - 20
        height: parent.height - 20
        spacing: 6

        //封面
        Image{
            id: cover
            source: "image://cover/file:" + musicLine.musicId.toString()
            sourceSize.height: height
            sourceSize.width: width
            width: 50
            height: 50
            cache: true
            asynchronous: true
        }

        Column{
            id: coreName
            width: (parent.width - tool.width - cover.width - 30) / 2

            TikoTextLine{
                text: musicLine.music.title
                exSize: 3
                font.bold: true
                height: 30
                width: parent.width
            }
            TikoTextLine{
                text: musicLine.music.artist
                height: 20
                width: parent.width
            }
        }

        TikoTextLine{
            id: alumb
            text: musicLine.music.alumb
            width: coreName.width
            height: 50
        }

        Row{
            id: tool
            spacing: 3

            TikoButtonIcon{
                text: qsTr("喜爱")
                width: 50
                height: 50
                onClicked: musicLine.music.isLove = !musicLine.music.isLove
                icon.source: musicLine.music.isLove ? "qrc:/image/love.png" : "qrc:/image/unlove.png"
                cache: true
            }
            Row{
                width: 100
                height: 50
                spacing: 1

                Repeater{
                    delegate: TikoButtonIcon{
                        width: 20
                        height: 50
                        hover: 0
                        cache: true
                        onClicked: musicLine.music.level = level
                        icon.source: musicLine.music.level >= level ? "qrc:/image/int.png" : "qrc:/image/unInt.png"
                    }
                    model: ListModel{
                        ListElement{level: 0}
                        ListElement{level: 1}
                        ListElement{level: 2}
                        ListElement{level: 3}
                        ListElement{level: 4}
                    }
                }
            }
            TikoButtonIcon{
                text: qsTr("更多")
                width: 50
                height: 50
                onClicked: CoreData.openMenuMusic(musicLine)
                icon.source: "qrc:/image/else.png"
                cache: true
            }
            TikoTextLine{
                text: musicLine.music.getStringTime()
                width: CoreData.timeWidth
                height: 50
            }
            TikoTextLine{
                text: musicLine.music.lastEdit
                width: CoreData.editTimeWidth
                height: 50
            }
        }
    }

    function play(){
        MediaPlayer.playTableMusic(tableId, listId)
    }
}
