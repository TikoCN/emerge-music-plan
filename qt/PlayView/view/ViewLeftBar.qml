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
                id: iconRepeater
                delegate: TikoButtonIcon {
                    height: width * 0.6
                    width: (toolItem.width - TikoSeit.normalMargins) / 2
                    id: iconButton
                    property int itemIndex: index
                    checked: CoreData.fixedPageIndex === itemIndex
                    icon.source: modelData.icon
                    icon.width: 32
                    icon.height: icon.width
                    bgOpacity: 0
                    onClicked: {
                        switch (itemIndex) {
                        case 0:
                            CoreData.stackDetail()
                            break
                        case 1:
                            CoreData.stackLibrary()
                            break
                        case 2:
                            CoreData.stackSeit()
                            break
                        case 3:
                            CoreData.stackRcommend()
                            break
                        }
                    }
                }

                model: [
                    {icon: "qrc:/image/home.svg"},
                    {icon: "qrc:/image/album.svg"},
                    {icon: "qrc:/image/seit.svg"},
                    {icon: "qrc:/image/recommend.svg"}
                ]
            }
        }

        Connections {
            target: CoreData
            function onFixedPageChanged() { setIconSelection(CoreData.fixedPageIndex) }
        }

        ColumnLayout {
            anchors.top: toolItem.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: TikoSeit.emphasizeMargins

            TikoButtonDefault{
                id: addPlaylistButton
                Layout.fillWidth: true
                textLine.text: qsTr("新建列表")
                icon.source: "qrc:/image/new.svg"
                onLeftClicked: openInputWindow()
                height: 30
                bgOpacity: 0
            }


            Repeater {
                    delegate: TikoButtonDefault {
                        Layout.fillWidth: true
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
                        onRightClicked: position => openPlaylistMenu(model.id, model.isDir, model.name, norMalButton, position)
                        height: 30
                        bgOpacity: 0
                    }

                    model: PlaylistModel {
                        id: playlistModel
                        type: PlaylistModel.AllModel
                        Component.onCompleted: {
                            TaskCenter.reloadData.connect(clear)
                        }
                    }
                }
        }
    }

    Component{
        id: editMusicListMenu
        MenuPlaylist {
        }
    }

    property var inputWindow: null

    Component {
        id: inputWindowComponent
        TikoInputWindow {
            title: qsTr("新建列表，请输入列表名")
            onAccept: {
                if (inputText === "") {
                    CoreData.sendErrorMsg(qsTr("列表名不能为空"))
                    return
                }

                if (PlaylistLibrary.allowName(inputText)) {
                    PlaylistLibrary.appendUser(inputText)
                    setNormalText()
                    playlistModel.clear()
                } else {
                    CoreData.sendErrorMsg(qsTr("列表名不可用"))
                }
            }
        }
    }

    Rectangle {
        id: moveBgItem
        parent: addPlaylistButton
        width: parent.width
        height: parent.height
        color: TikoSeit.theme.colorMaxTop
        opacity: 0.08
        radius: 10
    }

    function setRectBgParent(parent) {
        moveBgItem.parent = parent
    }

    function setIconSelection(index) {
        var iconButton = iconRepeater.itemAt(index)
        if (iconButton !== null) {
            setRectBgParent(iconButton)
        }
    }

    function openInputWindow() {
        if (inputWindow === null) {
            const ownerWindow = root.Window.window
            inputWindow = inputWindowComponent.createObject(null, {
                x: ownerWindow.x + (ownerWindow.width - 600) / 2,
                y: ownerWindow.y + (ownerWindow.height - 400) / 2
            })
        }
        if (inputWindow !== null) {
            inputWindow.open()
        }
    }

    function openPlaylistMenu(playlistId, isDir, name, item, position){
        if (editMusicListMenu.status === Component.Ready) {
            var menu = editMusicListMenu.createObject(parent, {
                                                          playlistId: playlistId,
                                                          isDir:isDir,
                                                          name:name
                                                      })
            menu.popupAt(item, position.x, position.y)
        }
        else {
            console.log(editMusicListMenu.errorString())
        }
    }

}
