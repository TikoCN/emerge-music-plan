import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: root
    property string showText: ""

    //左侧导航
    ScrollView{
        ScrollBar.vertical.visible: false
        ScrollBar.horizontal.visible: false
        height: parent.height
        anchors.right: parent.right
        anchors.margins: 10
        width: parent.width - 10

        Column{
            spacing: 5
            width: parent.width
            height: parent.height

            //切换到推荐
            TikoButtonNormal{
                width: parent.width
                id: mainPageButton
                textLine.text: qsTr("推荐")
                icon.source: "qrc:/image/main.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onLeftClicked: {
                    root.showText = textLine.text
                    mainView.turnToMain()
                }
            }

            TikoButtonNormal{
                id: artistButton
                width: parent.width
                textLine.text: qsTr("专辑")
                icon.source: "qrc:/image/album.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.showText = textLine.text
                    mainView.turnToAlbum()
                }
            }

            TikoButtonNormal{
                width: parent.width
                textLine.text: qsTr("作曲家")
                icon.source: "qrc:/image/artist.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    root.showText = textLine.text
                    mainView.turnToArtist()
                }
            }

            //切换到设置
            TikoButtonNormal{
                width: parent.width
                id: seitPageButton
                textLine.text: qsTr("设置")
                anchors.horizontalCenter: parent.horizontalCenter
                onLeftClicked:{
                    root.showText = textLine.text
                    mainView.turnToSeit();
                }
                icon.source: "qrc:/image/seit.png"
            }

            //新建列表
            TikoButtonNormal{
                width: parent.width
                id: addPlayListButton
                textLine.text: qsTr("新建列表")
                anchors.horizontalCenter: parent.horizontalCenter
                icon.source: "qrc:/image/new.png"
                onLeftClicked: inputName.open()

                TikoPopupInput {
                    anchors.centerIn: Overlay.overlay
                    id: inputName
                    text: qsTr("新建列表，请输入列表名")
                    onAccept: {
                        if (inputText === "") {
                            CoreData.sendErrorMsg("列表名不能为空")
                            return
                        }

                        if (SQLData.checkPlayListName(inputText)) {
                            DataActive.appendPlayList(inputText)
                            inputName.setNormalText()
                        }
                        else {
                            CoreData.sendErrorMsg("列表名不可用")
                        }
                    }
                }
            }

            // 插入的列表按钮
            ListView{
                width: parent.width
                id: userPlayListListView
                implicitHeight: childrenRect.height
                spacing: 5

                delegate: TikoButtonNormal{
                    width: userPlayListListView.width
                    textLine.text: name
                    icon.source: "image://cover/playlistFile?id=" +
                                playlistId.toString() +
                                "&radius=3"
                    onLeftClicked: {
                        root.showText = textLine.text
                        CoreData.mainTurnMusicList(playlistId)
                    }
                    onRightClicked: openPlayListMenu(playlistId, isDir, name)
                }

                model: ListModel{
                    id: userPlayListModel
                }
            }
        }
    }

    // 菜单
    Component{
        id: editMusicListMenu
        MenuPlayList {

        }
    }

    function openPlayListMenu(playlistId, isDir, name){
        if (editMusicListMenu.status === Component.Ready) {
            var menu = editMusicListMenu.createObject(parent, {
                                                          playlistId: playlistId,
                                                          isDir:isDir,
                                                          name:name
                                                      })
            menu.popup()
        }
        else {
            console.log(editMusicListMenu.errorString())
        }
    }

    Connections{
        target: CoreData
        function onPlaylistChanged(){
            userPlayListModel.clear()
            var length = CoreData.playlist.length

            for (var i=0; i<length; i++){
                userPlayListModel.append({
                                          playlistId: CoreData.playlist[i]["playlistId"],
                                          name: CoreData.playlist[i]["name"],
                                          isDir: CoreData.playlist[i]["isDir"]
                                      })

            }
        }
    }
}
