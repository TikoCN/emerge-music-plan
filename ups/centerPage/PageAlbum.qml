import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import Ups

Item {
    id: albumPage

    property string key: ""
    property string keyList: []

    // 跳转按钮列表
    ListView {
        id: albumButtonList
        orientation: ListView.Horizontal
        width: parent.width - 60
        currentIndex: keyList.indexOf(key)
        height: 40
        anchors.left: parent.left
        anchors.margins: 30
        spacing: 10
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4

        model: ListModel {
            id: albumButtonModel
        }

        delegate: TikoTextLine {
            text: keyString
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3
            exSize: ListView.isCurrentItem ? 5 : 3

            MouseArea {
                anchors.fill: parent
                onClicked: key = keyString
            }
        }
    }

    Grid {
        id: albumShow
        width: parent.width
        columns: width / 160
        columnSpacing: 9
        rowSpacing: 10
        anchors.top: albumButtonList.bottom
        anchors.margins: 30
        flow: Grid.LeftToRight

        Repeater{
            model: ListModel {
                id: albumModel
            }

            delegate: CoreAlbumButton {
                id: albumButton
                albumId: inalbumId
            }
        }
    }


    function build () {
        albumModel.clear()
        var list = SQLData.getAlbumKeyList()
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (var i=0; i<list.length; i++) {
            albumButtonModel.append({keyString: list[i]})
        }
    }

    onKeyChanged: {
        albumModel.clear()
        var list = SQLData.getAlbum(key)
        for (var i=0; i<list.length; i++) {
            albumModel.append({inalbumId: list[i]})
        }
    }
}


