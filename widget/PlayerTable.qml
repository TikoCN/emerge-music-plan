import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import TikoAPI
import Tiko

Item {
    id: root
    property int tableId//歌曲标题
    property int allMusic:  0
    property string showCover: "qrc:/image/default.jpg"


    Item{
        id: showView
        width: parent.width
        height: childrenRect.height

        //列表封面
        Image{
            id: tableCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            source: root.showCover
            sourceSize.width: width
            sourceSize.height: height
            width: 200
            height: 200
            cache: false
            asynchronous: true
        }

        //列表名字
        TikoAutoText{
            id: tableName
            anchors.left: tableCover.right
            anchors.leftMargin: 10
            anchors.top: tableCover.top
            text: MediaPlayer.tableList[tableId].name
            width: parent.width - tableCover.width
            height: tableCover.height - playButton.height
            exSize: 20
            font.bold: true
            horizontalAlignment: Text.AlignHCenter
        }

        //播放列表
        TikoBorderButton{
            id: playButton
            anchors.left: tableName.left
            anchors.bottom: tableCover.bottom
            text: qsTr("播放")
            icon.source: "qrc:/image/play.png"
            width: 100
            onClicked: MediaPlayer.playMusic(tableId, 0)

        }

        //批量操作
        TikoBorderButton{
            anchors.left: playButton.right
            anchors.leftMargin: 10
            anchors.bottom: tableCover.bottom
            text: qsTr("批量")
            icon.source: "qrc:/image/batch.png"
            width: 100
        }

        RowLayout{
            id: tableTool
            anchors.top: tableCover.bottom
            anchors.topMargin: 10
            anchors.left: tableCover.left
            anchors.leftMargin: 10
            spacing: 10

            TikoButton{
                Layout.minimumWidth: 70
                onClicked: MediaPlayer.tableList[tableId].showAllMusic()
                text: qsTr("歌曲") + allMusic.toString()
                icon.source: "qrc:/image/music.png"
            }

            TikoButton{
                Layout.minimumWidth: 70
                onClicked: MediaPlayer.tableList[tableId].showLove()
                text: qsTr("喜爱")
                icon.source: "qrc:/image/love.png"
            }

            TikoButton{
                Layout.minimumWidth: 70
                onClicked: MediaPlayer.tableList[tableId].openDir()
                text: qsTr("打开")
                icon.source: "qrc:/image/dir.png"
            }

            TikoButton{
                Layout.minimumWidth: 70
                onClicked: editName.open()
                text: qsTr("命名")
                icon.source: "qrc:/image/rename.png"

                TikoPopupInput{
                    id: editName
                    text: qsTr("请输入列表的新名字")
                    orgText: MediaPlayer.tableList[tableId].name
                    onAccept: ()=>{
                                  MediaPlayer.tableList[tableId].name = inputText
                              }
                }
            }

            TikoButton{
                Layout.minimumWidth: 70
                onClicked: sortMenu.open()
                text: qsTr("排序")
                icon.source: "qrc:/image/sort.png"

                TikoMenu{
                    id: sortMenu
                    width: parent.width
                    y: parent.height

                    TikoMenuItem{
                        text: qsTr("标   题");
                        onTriggered: MediaPlayer.tableList[tableId].key = 0;
                        icon.source: MediaPlayer.tableList[tableId].key === 0 ? "qrc:/image/used.png" : ""}
                    TikoMenuItem{
                        text: qsTr("歌   手");
                        onTriggered: MediaPlayer.tableList[tableId].key = 1;
                        icon.source: MediaPlayer.tableList[tableId].key === 1 ? "qrc:/image/used.png" : ""}
                    TikoMenuItem{
                        text: qsTr("专   辑");
                        onTriggered: MediaPlayer.tableList[tableId].key = 2;
                        icon.source: MediaPlayer.tableList[tableId].key === 2 ? "qrc:/image/used.png" : ""}
                    TikoMenuItem{
                        text: qsTr("时   长");
                        onTriggered: MediaPlayer.tableList[tableId].key = 3;
                        icon.source: MediaPlayer.tableList[tableId].key === 3 ? "qrc:/image/used.png" : ""}
                    TikoMenuItem{
                        text: qsTr("修改时间");
                        onTriggered: MediaPlayer.tableList[tableId].key = 4;
                        icon.source: MediaPlayer.tableList[tableId].key === 4 ? "qrc:/image/used.png" : ""}
                    TikoMenuSpeacer{}
                    TikoMenuItem{
                        text: qsTr("升   序");
                        onTriggered: MediaPlayer.tableList[tableId].forward = false;
                        icon.source: MediaPlayer.tableList[tableId].forward === false ? "qrc:/image/used.png" : ""}
                    TikoMenuItem{
                        text: qsTr("降   序");
                        onTriggered: MediaPlayer.tableList[tableId].forward = true;
                        icon.source: MediaPlayer.tableList[tableId].forward === true ? "qrc:/image/used.png" : ""}
                }
            }
        }

        Rectangle{
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: tableTool.verticalCenter
            width: 150
            height: 40
            color: Setting.themeColor
            radius: 10
            opacity: 0.3

            //搜索
            TikoInputText{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                width: parent.width - 20 - 25
                show.text: qsTr("搜索：")
                borderColor: "#00000000"
                input.onEditingFinished: {
                    if(MediaPlayer.tableList[tableId].search !== input.text){
                        MediaPlayer.tableList[tableId].search = input.text
                    }
                }
            }

            TikoUiButton{
                text: qsTr("搜索")
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 20
                icon.source: "qrc:/image/search.png"
            }
        }
    }

    ScrollView{
        width: parent.width
        height: parent.height - showView.height - 20
        anchors.top: showView.bottom
        anchors.topMargin: 20
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        ScrollBar.horizontal.visible: false
        ScrollBar.vertical: TikoBar{}

        ListView{
            id: musicList
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
            spacing: 15
            clip: true

            model: ListModel{
                id: musicModel
            }

            delegate: CoreMusicLine{
                width: musicList.width - 20
                tableId: table
                listId: music
                core: musicCore
            }
        }
    }

    Connections{
        target: MediaPlayer.tableList[tableId]
        function onShowMusicsChanged(){
            //重新建立条目
            musicModel.clear()
            for(let i=0; i<MediaPlayer.tableList[tableId].showMusics.length; i++){
                musicModel.append({music: i,
                                      table: root.tableId,
                                      musicCore: MediaPlayer.tableList[tableId].showMusics[i]})
            }
        }

        function onAddMusic(size){
            musicModel.clear()
            for(var i=0; i<size; i++){
                var music = MediaPlayer.tableList[tableId].showMusics.length - size + i
                musicModel.append({music: music,
                                      table: root.tableId,
                                      musicCore: MediaPlayer.tableList[tableId].showMusics[music]})
            }

            //调整列表展示信息
            if(MediaPlayer.tableList[tableId].showMusics.length !== 0){
                var coorId = MediaPlayer.tableList[root.tableId].getLastCoreId()
                root.showCover = "image://cover/file:" + coorId.toString()
                root.allMusic = MediaPlayer.tableList[root.tableId].musics.length
            }
        }
    }
}
