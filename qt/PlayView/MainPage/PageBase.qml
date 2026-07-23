import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import MediaerAPI
import Tiko
import PlayView
import DataType

Item {
    id: playerPlayList
    property int sort: -1
    property int duration: -1
    property int musicCount: 0
    property string name: ""
    property string search: ""
    property string loadIcon: ""
    property string normalIcon: ""
    property ListViewMusic musicList: musicListView

    signal createMenu()

    Item{
        id: showView
        width: parent.width
        height: childrenRect.height

        //列表封面
        AutoCoverImage {
            id: playlistCover
            anchors.left: parent.left
            anchors.leftMargin: 10
            normalUrl: normalIcon
            baseUrl: loadIcon
            width: 100
            height: 100
        }

        //列表名字
        TikoTextLine{
            id: playlistName
            anchors.left: playlistCover.right
            anchors.leftMargin: 10
            anchors.top: playlistCover.top
            text: name
            width: parent.width - playlistCover.width
            level: 2
        }

        // 列表信息
        TikoTextLine{
            id: playlistHelp
            anchors.left: playlistName.left
            anchors.top: playlistName.bottom
            anchors.topMargin: 10
            width: parent.width - playlistCover.width
            text: qsTr("包含 %1 首歌曲，时长为：%2").arg(musicCount).arg(BaseTool.typeConversion.durationToTimeStringNoMax(duration))
            level: 0
        }

        RowLayout{
            id: playlistTool
            anchors.top: playlistCover.bottom
            anchors.topMargin: 10
            anchors.left: playlistCover.left
            anchors.leftMargin: 10
            spacing: 10

            //显示所有歌曲列表
            TikoButtonDefault{
                Layout.minimumWidth: 70
                textLine.text: qsTr("歌曲") + musicCount.toString()
                icon.source: "qrc:/image/music.png"
                onLeftClicked: {
                    musicListView.reset()
                }
            }

            //显示喜爱歌曲列表
            TikoButtonDefault{
                Layout.minimumWidth: 70
                textLine.text: qsTr("喜爱")
                icon.source: "qrc:/image/love.png"
                onLeftClicked: {
                    musicListView.onlyLove = !musicListView.onlyLove
                    musicListView.reset()
                }
            }

            //排序
            TikoButtonDefault{
                Layout.minimumWidth: 70
                onLeftClicked: sort()
                textLine.text: qsTr("排序")
                icon.source: "qrc:/image/sort.png"

                Component {
                    id: sortMenuComponent
                    MenuPlayListSort {
                        playlistId: playerPlayList.playlistId
                        sort: playerPlayList.sort
                        onNewSortChanged: {
                            playerPlayList.sort = newSort
                            musicListView.reset()
                        }
                    }
                }

                function sort(){
                    if (sortMenuComponent.status === Component.Ready){
                        const menu = sortMenuComponent.createObject(this);
                        menu.popup()
                    }
                }
            }


            //批量操作
            TikoButtonDefault{
                textLine.text: qsTr("批量")
                Layout.minimumWidth: 70
                onClicked: {}
            }

            //搜索
            TikoTextInput{
                id: inputText
                Layout.minimumWidth: 150
                placeholderText: qsTr("搜索")
                onEditingFinished:{
                    search = text
                    musicListView.reset()
                }

                background: Rectangle {
                    color: TikoSeit.theme.colorBgView
                    border.color: TikoSeit.theme.colorBgHint
                    border.width: 0.3
                    radius: 10
                }
            }
        }
    }

    ListViewMusic{
        id: musicListView
        anchors.top: showView.bottom
        anchors.bottom: playerPlayList.bottom
        anchors.left: playerPlayList.left
        anchors.right: playerPlayList.right
        anchors.margins: TikoSeit.emphasizeMargins
    }
}
