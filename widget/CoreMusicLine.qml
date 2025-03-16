import QtQuick
import QtQuick.Layouts
import TikoAPI
import Tiko

Item {
    id:root
    implicitHeight: 70
    implicitWidth: 100
    clip: true

    property int tableId
    property int listId
    property int coreId: core.coreId
    property var core

    MouseArea{
        onClicked:(mouse)=>{
            if(mouse.button === Qt.RightButton){
                rightMenu.popup()
            }
            else{
                root.play()
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
            if(root.listId % 2 === 1){
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
        spacing: 10

        //封面
        Image{
            id: cover
            source: "image://cover/file:" + root.coreId.toString()
            sourceSize.height: height
            sourceSize.width: width
            width: 50
            height:50
            cache: true
            asynchronous: true
        }

        Column{
            id: coreName
            width: (parent.width - tool.width - cover.width) / 2

            TikoTextLine{
                text: root.core.title
                exSize: 3
                font.bold: true
                height: 30
                width: parent.width
            }
            TikoTextLine{
                text: root.core.artist
                height: 20
                width: parent.width
            }
        }

        TikoTextLine{
            id: alumb
            text: root.core.alumb
            width: coreName.width
            height: 50
        }

        Row{
            id: tool
            spacing: 10

            TikoButtonIcon{
                text: qsTr("更多")
                width: 50
                height: 50
                onClicked: rightMenu.open()
                icon.source: "qrc:/image/else.png"
            }
            TikoTextLine{
                text: root.core.getStringTime()
                width: CoreData.timeWidth
                height: 50
            }
            TikoTextLine{
                text: root.core.lastEdit
                width: CoreData.editTimeWidth
                height: 50
            }
        }
    }

    MenuMusic{
        id: rightMenu
    }

    function play(){
        MediaPlayer.playMusic(tableId, listId)
    }
}
