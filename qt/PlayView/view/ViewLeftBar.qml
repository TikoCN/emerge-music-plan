import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import PlayView

Item {
    id: root
    property string showText: ""

    signal turnToMain()
    signal turnToArtist()
    signal turnToAlbum()
    signal turnToSeit()

    //左侧导航
    ScrollView{
        ScrollBar.vertical.visible: false
        ScrollBar.horizontal.visible: false
        height: parent.height
        anchors.right: parent.right
        anchors.margins: 10
        width: parent.width - 10

        Grid {
            id: toolItem
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins
            columns: 2
            spacing: TikoSeit.normalMargins

            Repeater {
                delegate: Rectangle {
                    height: width * 0.6
                    width: (toolItem.width - TikoSeit.normalMargins) / 2
                    color: TikoSeit.theme.baseTheme.transparentNormal
                    radius: 10

                    TikoButtonIconNormal {
                        id: iconButton
                        anchors.centerIn: parent
                        icon.source: modelData.icon
                        onClicked: {
                            root.showText = modelData.icon
                            modelData.click()
                        }
                    }
                }

                model: [
                    {icon: "qrc:/image/main.png", click: root.turnToMain},
                    {icon: "qrc:/image/album.png", click: root.turnToAlbum},
                    {icon: "qrc:/image/artist.png", click: root.turnToArtist},
                    {icon: "qrc:/image/seit.png", click: root.turnToSeit}
                ]
            }
        }


        ColumnLayout {
            anchors.top: toolItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins

            //新建列表
            TikoButtonNormal{
                id: addPlayListButton
                textLine.text: qsTr("新建列表")
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
            Repeater {
                id: userPlayListListView

                delegate: Rectangle {
                    width: parent.width
                    height: norMalButton.implicitHeight + TikoSeit.emphasizeMargins
                    radius: 10
                    color: root.showText === norMalButton.textLine.text ? TikoSeit.theme.baseTheme.transparentEmphasize : "#00000000"

                    TikoButtonNormal{
                        id: norMalButton
                        anchors.fill: parent
                        icon.anchors.leftMargin: TikoSeit.emphasizeMargins
                        icon.enableUnifiedColor: false
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
