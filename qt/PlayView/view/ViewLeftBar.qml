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

    ScrollView{
        id: scrollview
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
                        setRectBgParent(iconButton)
                        modelData.click()
                    }
                }

                model: [
                    {icon: "qrc:/image/home.png", click: CoreData.stackDetail},
                    {icon: "qrc:/image/album.png", click: CoreData.stackLibrary},
                    {icon: "qrc:/image/seit.png", click: CoreData.stackSeit},
                    {icon: "qrc:/image/seit.png", click: CoreData.stackRcommend}
                ]
            }
        }


        ColumnLayout {
            anchors.top: toolItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins

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

                        if (PlayListLibrary.allowName(inputText)) {
                            PlayListLibrary.appendUser(inputText)
                            inputName.setNormalText()
                            updatePlayLists()
                        }
                        else {
                            CoreData.sendErrorMsg("列表名不可用")
                        }
                    }
                }
            }


            Repeater {
                    delegate: TikoButtonDefault {
                        width: scrollview.width
                        id: norMalButton
                        icon.anchors.leftMargin: TikoSeit.emphasizeMargins
                        icon.enableUnifiedColor: false
                        textLine.text: model.name
                        icon.source: "image://cover/playlistFile?id=" +
                                     model.id.toString() +
                                     "&radius=3"
                        onLeftClicked: {
                            CoreData.stackPlaylist(model.id)
                            setRectBgParent(norMalButton)
                        }
                        onRightClicked: openPlayListMenu(model.id, model.isDir, model.name)
                        height: 30
                        bgOpacity: 0
                    }

                    model: PlaylistModel {
                        id: playlistModel
                        type: PlaylistModel.All
                    }
                }
        }
    }

    Component{
        id: editMusicListMenu
        MenuPlayList {
        }
    }

    Rectangle {
        id: moveBgItem
        parent: addPlayListButton
        width: parent.width
        height: parent.height
        color: TikoSeit.theme.colorMaxTop
        opacity: 0.08
        radius: 10
    }

    function setRectBgParent(parent) {
        moveBgItem.parent = parent
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

}