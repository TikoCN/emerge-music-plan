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

        background: Rectangle{
            width: parent.width
            height: parent.height
            color: Setting.transparentColor//背景颜色
            opacity: 0.01
        }

        Column{
            spacing: 5
            width: parent.width
            height: parent.height

            //切换到推荐
            TikoButtonNormal{
                width: parent.width
                id: mainPageButton
                text: qsTr("推荐")
                iconSource: "qrc:/image/main.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClickLeft: {
                    root.showText = text
                    mainView.turnToMain()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            TikoButtonNormal{
                id: artistButton
                width: parent.width
                text: qsTr("专辑")
                iconSource: "qrc:/image/album.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClick: {
                    root.showText = text
                    mainView.turnToAlbum()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            TikoButtonNormal{
                width: parent.width
                text: qsTr("作曲家")
                iconSource: "qrc:/image/artist.png"
                anchors.horizontalCenter: parent.horizontalCenter
                onClick: {
                    root.showText = text
                    mainView.turnToArtist()
                }

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            //切换到设置
            TikoButtonNormal{
                width: parent.width
                id: seitPageButton
                text: qsTr("设置")
                anchors.horizontalCenter: parent.horizontalCenter
                onClickLeft:{
                    root.showText = text
                    mainView.turnToSeit();
                }
                iconSource: "qrc:/image/seit.png"

                Rectangle {
                    color: TikoSeit.transparentColor
                    opacity: 0.2
                    visible: root.showText === parent.text
                    anchors.fill: parent
                    radius: 15
                }
            }

            //新建列表
            TikoButtonNormal{
                width: parent.width
                id: addPlayListButton
                text: qsTr("新建列表")
                anchors.horizontalCenter: parent.horizontalCenter
                iconSource: "qrc:/image/new.png"
                onClickLeft: inputName.open()

                TikoPopupInput{
                    id: inputName
                    text: qsTr("新建列表，请输入列表名")
                    onAccept: {
<<<<<<< Updated upstream:PlayView/view/ViewLeftBar.qml
                        Core.appendTable(inputName.inputText)
                        inputName.setNormalText()
=======
                        if (inputText === "") {
                            CoreData.sendErrorMsg("列表名不能为空")
                            return
                        }

                        if (SQLData.checkPlayListName(inputText)) {
                            Core.appendPlayList(inputText)
                            inputName.setNormalText()
                        }
                        else {
                            CoreData.sendErrorMsg("列表名不可用")
                        }
>>>>>>> Stashed changes:qt/PlayView/view/ViewLeftBar.qml
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
                    text: name
<<<<<<< Updated upstream:PlayView/view/ViewLeftBar.qml
                    iconSource: "image://cover/tableFile:" + tableId.toString()
=======
                    iconSource: "image://cover/playlistFile?id=" +
                                playlistId.toString() +
                                "&radius=3"
>>>>>>> Stashed changes:qt/PlayView/view/ViewLeftBar.qml
                    iconWidth: 30
                    iconHeight: 30
                    cache: false
                    useAutoColor: false
                    onClickLeft: {
                        root.showText = text
<<<<<<< Updated upstream:PlayView/view/ViewLeftBar.qml
                        mainView.stackTable(tableId)
                    }
                    onClickRight: tableMenu.open()
=======
                        CoreData.mainTurnMusicList(playlistId)
                    }
                    onClickRight: openPlayListMenu(playlistId, isDir, name)
>>>>>>> Stashed changes:qt/PlayView/view/ViewLeftBar.qml

                    Rectangle {
                        color: TikoSeit.transparentColor
                        opacity: 0.2
                        visible: root.showText === parent.text
                        anchors.fill: parent
                        radius: 15
                    }
                }

                model: ListModel{
                    id: userPlayListModel
                }
            }
        }
    }

<<<<<<< Updated upstream:PlayView/view/ViewLeftBar.qml
=======
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

>>>>>>> Stashed changes:qt/PlayView/view/ViewLeftBar.qml
    Connections{
        target: CoreData
        function onPlaylistChanged(){
            userPlayListModel.clear()
            var length = CoreData.playlist.length

            for (var i=0; i<length; i++){
<<<<<<< Updated upstream:PlayView/view/ViewLeftBar.qml
                userTableModel.append({
                                          tableId: CoreData.table[i]["tableId"],
                                          name: CoreData.table[i]["name"]
=======
                userPlayListModel.append({
                                          playlistId: CoreData.playlist[i]["playlistId"],
                                          name: CoreData.playlist[i]["name"],
                                          isDir: CoreData.playlist[i]["isDir"]
>>>>>>> Stashed changes:qt/PlayView/view/ViewLeftBar.qml
                                      })

            }
        }
    }
}
