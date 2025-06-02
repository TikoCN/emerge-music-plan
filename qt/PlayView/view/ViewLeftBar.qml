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
                id: addTableButton
                text: qsTr("新建列表")
                anchors.horizontalCenter: parent.horizontalCenter
                iconSource: "qrc:/image/new.png"
                onClickLeft: inputName.open()

                TikoPopupInput {
                    anchors.centerIn: Overlay.overlay
                    id: inputName
                    text: qsTr("新建列表，请输入列表名")
                    onAccept: {
                        if (inputText === "") {
                            CoreData.sendErrorMsg("列表名不能为空")
                            return
                        }

                        if (SQLData.checkTableName(inputText)) {
                            Core.appendTable(inputText)
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
                id: userTableListView
                implicitHeight: childrenRect.height
                spacing: 5

                delegate: TikoButtonNormal{
                    width: userTableListView.width
                    text: name
                    iconSource: "image://cover/tableFile?id=" + tableId.toString() + "&raidus=3"
                    iconWidth: 30
                    iconHeight: 30
                    cache: false
                    useAutoColor: false
                    onClickLeft: {
                        root.showText = text
                        CoreData.mainTurnMusicList(tableId)
                    }
                    onClickRight: openMenu()

                    Rectangle {
                        color: TikoSeit.transparentColor
                        opacity: 0.2
                        visible: root.showText === parent.text
                        anchors.fill: parent
                        radius: 15
                    }
                }

                model: ListModel{
                    id: userTableModel
                }
            }
        }
    }

    // 菜单
    Component{
        id: editMusicListMenu
        MenuTable {
        }
    }

    function openMenu(tableId){
        if (editMusicListMenu.status === Component.Ready) {
            var menu = editMusicListMenu.createObject(parent, {tableId: tableId})
            menu.open()
        }
        else {
            console.log(editMusicListMenu.errorString())
        }
    }

    Connections{
        target: CoreData
        function onTableChanged(){
            userTableModel.clear()
            var length = CoreData.table.length

            for (var i=0; i<length; i++){
                userTableModel.append({
                                          tableId: CoreData.table[i]["tableId"],
                                          name: CoreData.table[i]["name"],
                                          isDir: CoreData.table[i]["isDir"]
                                      })

            }
        }
    }
}
