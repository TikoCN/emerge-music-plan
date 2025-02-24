import QtQuick
import QtQuick.Layouts
import MyAPI
import "../base"

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
        anchors.fill:parent
        radius:5
        color:Setting.transparentColor//背景颜色
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

            MyAutoText{
                text: root.core.title
                exSize: 3
                font.bold: true
                height: 30
                width: parent.width
            }
            MyAutoText{
                text: root.core.artist
                height: 20
                width: parent.width
            }
        }

        MyAutoText{
            id: alumb
            text: root.core.alumb
            width: coreName.width
            height: 50
        }

        Row{
            id: tool
            spacing: 10

            MyUiButton{
                text: qsTr("喜欢")
                width: 50
                height: 50
                onClicked: root.core.love = !root.core.love
                icon.source:{
                    if(root.core.love){
                        return "qrc:/image/love.png"
                    }
                    else{
                        return "qrc:/image/unlove.png"
                    }
                }
            }
            MyUiButton{
                text: qsTr("更多")
                width: 50
                height: 50
                onClicked: rightMenu.open()
                icon.source: "qrc:/image/else.png"
            }

            MyAutoText{
                text: root.core.getStringTime()
                width: Setting.timeWidth
                height: 50
            }
            MyAutoText{
                text: root.core.lastEdit
                width: Setting.editTimeWidth
                height: 50
            }
            MyAutoText{
                text: root.core.playNumber.toString()
                width: Setting.playNumberWidth
                height: 50
            }
        }
    }

    MyMenu{
        id: rightMenu
        cascade: true

        MyMenuItem{
            text: qsTr("播放")
            onTriggered: root.play()
            icon.source: "qrc:/image/playBlack.png"
        }

        MyMenuItem{
            text: qsTr("下一首播放")
            onTriggered: MediaPlayer.playingInsertMusic(coreId)
        }

        MyMenuItem{
            text: qsTr("添加到播放")
            onTriggered: MediaPlayer.musicInsertPlayingTable(coreId)
        }

        MyMenuSpeacer{}

        MyMenuItem{
            text: qsTr("喜欢")
            icon.source: "qrc:/image/love.png"
        }

        MyMenuItem{
            text: qsTr("打开文件夹")
            onTriggered: root.core.openMusicDir()
            icon.source: "qrc:/image/dir.png"
        }

        MyMenuItem{
            text: qsTr("打开歌词")
            onTriggered: root.core.openMusicLrc()
        }

        MyMenuItem{
            text: qsTr("打开封面")
            onTriggered: root.core.openMusicCover()
        }

        MyMenuSpeacer{}

        MyMenuItem{
            text: qsTr("复制 文件路径")
            onTriggered: root.core.copyMusicUrl()
        }

        MyMenuItem{
            text: qsTr("复制 歌曲信息")
            onTriggered: root.core.copyMusicData()
        }

        MyMenuSpeacer{}

        MyMenuItem{
            text: qsTr("编辑音乐信息")
            onTriggered: ToolHelper.editMusic(root.coreId)
        }

        MyMenu{
            title: qsTr("从网络中下载")
            MyMenuItem{
                text: qsTr("从 网络中下载 封面")
                onTriggered: OnLine.downCover(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 QQ音乐 下载 封面")
                onTriggered: OnLine.downCoverFromQQMusic(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 网易云 下载 封面")
                onTriggered: OnLine.downCoverFromNetEase(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 Bing搜索 下载 封面")
                onTriggered: OnLine.downCoverFromBing(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 Baidu搜索 下载 封面")
                onTriggered: OnLine.downCoverFromBaidu(core.getSearchString(), core.url);
            }
            MyMenuSpeacer{}
            MyMenuItem{
                text: qsTr("从 网络中下载 歌词")
                onTriggered: OnLine.downLrc(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 QQ音乐 下载 歌词")
                onTriggered: OnLine.downLrcFromQQMusic(core.getSearchString(), core.url);
            }
            MyMenuItem{
                text: qsTr("从 网易云 下载 歌词")
                onTriggered: OnLine.downLrcFromNetEase(core.getSearchString(), core.url);
            }
        }

        MyMenu{
            title: qsTr("格式转换为")
            Repeater{
                delegate: MyMenuItem{
                    text: suffix
                    onTriggered: core.setSuffix(suffix);
                }
                model: ListModel{
                    ListElement{suffix: "MP3"}
                    ListElement{suffix: "FLAC"}
                    ListElement{suffix: "ALAC"}
                    ListElement{suffix: "AAC"}
                    ListElement{suffix: "WMA"}
                    ListElement{suffix: "PCM16"}
                    ListElement{suffix: "PCM32"}

                }
            }
        }

        MyMenuSpeacer{}

        MyMenu{
            title: qsTr("添加到")
            icon.source: "qrc:/image/move.png"

            Repeater{
                delegate: addMenu
                model: ListModel{
                    id: addMenuData
                }
            }
            Component {
                id:  addMenu
                MyMenuItem {
                    text: MediaPlayer.tableList[aim].name
                    onTriggered: MediaPlayer.tableList[aim].insertMusic(core)
                }
            }

            onOpened: {
                addMenuData.clear()
                for(var i=0; i<MediaPlayer.tableList.length; i++){
                    addMenuData.append({aim: i})
                }
            }
        }

        MyMenu{
            title: qsTr("移动到")
            icon.source: "qrc:/image/addR.png"

            Repeater{
                delegate: moveMenu
                model: ListModel{
                    id: moveMenuData
                }
            }
            Component {
                id:  moveMenu
                MyMenuItem {
                    text:  MediaPlayer.tableList[aim].name
                    onTriggered: MediaPlayer.tableMoveMusic(root.tableId, root.listId, aim)
                }
            }

            onOpened: {
                moveMenuData.clear()
                for(var i=0; i<MediaPlayer.tableList.length; i++){
                    moveMenuData.append({aim: i})
                }
            }
        }
    }

    function play(){
        MediaPlayer.playMusic(tableId, listId)
    }
}
