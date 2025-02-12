import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MyAPI

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
        MyAutoText{
            id: tableName
            anchors.left: tableCover.right
            anchors.leftMargin: 10
            anchors.top: tableCover.top
            text: MediaPlayer.tableList[tableId].name
            width: parent.width
            exSize: 20
            font.bold: true
            height: 50
        }

        //播放列表
        MyBarButton{
            id: playButton
            anchors.left: tableName.left
            anchors.bottom: tableCover.bottom
            text: qsTr("播放")
            normal: 0.1
            icon.source: "qrc:/image/play.png"
            width: 100
            onClicked: MediaPlayer.playMusic(tableId, 0)
        }

        //批量操作
        MyBarButton{
            anchors.left: playButton.right
            anchors.leftMargin: 10
            anchors.bottom: tableCover.bottom
            text: qsTr("批量")
            normal: 0.1
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

            Repeater{
                delegate: MyBarButton{
                    Layout.minimumWidth: 90
                    onClicked: tableTool.work(i)
                    text: name
                    icon.source: icons
                }

                model: ListModel{
                    ListElement{name: qsTr("歌曲"); icons:"qrc:/image/music.png" ;i:1}
                    ListElement{name: qsTr("喜爱"); icons:"qrc:/image/love.png" ;i:2}
                    ListElement{name: qsTr("打开"); icons:"qrc:/image/dir.png" ;i:3}
                    ListElement{name: qsTr("排序"); icons:"qrc:/image/sort.png" ;i:4}
                    ListElement{name: qsTr("命名"); icons:"qrc:/image/rename.png" ;i:5}
                }
            }

            MyMenu{
                id: sortMenu

                MyMenuItem{text: qsTr("标   题"); onTriggered: MediaPlayer.tableList[tableId].key = 0; icon.source: sortMenu.isSelectKey(0)}
                MyMenuItem{text: qsTr("歌   手"); onTriggered: MediaPlayer.tableList[tableId].key = 1; icon.source: sortMenu.isSelectKey(1)}
                MyMenuItem{text: qsTr("专   辑"); onTriggered: MediaPlayer.tableList[tableId].key = 2; icon.source: sortMenu.isSelectKey(2)}
                MyMenuItem{text: qsTr("时   长"); onTriggered: MediaPlayer.tableList[tableId].key = 3; icon.source: sortMenu.isSelectKey(3)}
                MyMenuItem{text: qsTr("修改时间"); onTriggered: MediaPlayer.tableList[tableId].key = 4; icon.source: sortMenu.isSelectKey(4)}

                MyMenuItem{text: qsTr("升   序"); onTriggered: MediaPlayer.tableList[tableId].forward = false; icon.source: sortMenu.isSelectForward(false)}
                MyMenuItem{text: qsTr("降   序"); onTriggered: MediaPlayer.tableList[tableId].forward = true; icon.source: sortMenu.isSelectForward(true)}

                //被选中的键
                function isSelectKey(key){
                    if(MediaPlayer.tableList[tableId].key === key){
                        return "qrc:/image/used.png"
                    }
                }

                //被选中的键
                function isSelectForward(forward){
                    if(MediaPlayer.tableList[tableId].forward === forward){
                        return "qrc:/image/used.png"
                    }
                }
            }

            PopupInput{
                id: editName
                text: qsTr("请输入列表的新名字")
                orgText: MediaPlayer.tableList[tableId].name
                onAccept: ()=>{
                              MediaPlayer.tableList[tableId].name = inputText
                          }
            }

            function work(type){
                if(type === 1){MediaPlayer.tableList[tableId].showAllMusic()}
                else if(type === 2){MediaPlayer.tableList[tableId].showLove()}
                else if(type === 3){MediaPlayer.tableList[tableId].openDir()}
                else if(type === 4){sortMenu.open()}
                else if(type === 5){editName.open()}
            }
        }

        Rectangle{
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.verticalCenter: tableTool.verticalCenter
            width: 200
            height: 40
            color: Setting.themeColor
            radius: 15
            opacity: 0.3

            //搜索
            MyInputText{
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 20
                width: parent.width - 20 - 25
                show.text: qsTr("搜索：")
                input.onEditingFinished: {
                    if(MediaPlayer.tableList[tableId].search !== input.text){
                        MediaPlayer.tableList[tableId].search = input.text
                    }
                }
            }

            MyUiButton{
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
        ScrollBar.vertical: MyBar{}

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
