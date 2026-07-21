import QtQuick
import QtQuick.Layouts
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI   
import Tiko
import PlayView
import QtQuick.Effects

Item {
    id: root
    property string showText: "qrc:/image/seit.png"


    Rectangle {
        anchors.fill: parent
        color: TikoSeit.theme.colorBgView
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: TikoSeit.theme.colorBgHint
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }
        radius: 10
    }

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
                delegate: TikoButtonIcon {
                    height: width * 0.6
                    width: (toolItem.width - TikoSeit.normalMargins) / 2
                    id: iconButton
                    icon.source: modelData.icon
                    icon.width: 32
                    icon.height: icon.width
                    bgOpacity: 0
                    onClicked: {
                        root.showText = modelData.icon
                        modelData.click()
                    }

                }

                model: [
                    {icon: "qrc:/image/home.png", click: CoreData.mainTurnMain},
                    {icon: "qrc:/image/album.png", click: CoreData.mainTurnLibraryPage},
                    {icon: "qrc:/image/seit.png", click: CoreData.mainTurnSeit}
                ]
            }
        }


        ColumnLayout {
            anchors.top: toolItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins

            //新建列表
            TikoButtonDefault{
                id: addPlayListButton
                textLine.text: qsTr("新建列表")
                icon.source: "qrc:/image/new.png"
                onLeftClicked: inputName.open()
                height: 30
                bgOpacity: 0

                TikoPopupInput {
                    anchors.centerIn: Overlay.overlay
                    id: inputName
                    title: qsTr("新建列表，请输入列表名")
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

                delegate: TikoButtonDefault {
                    id: norMalButton
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
                    height: 30
                    bgOpacity: 0
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
