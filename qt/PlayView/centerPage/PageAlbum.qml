import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import PlayView

Item {
    id: albumPage

    property string key: ""
    property string keyList: []

    // 跳转按钮列表
    ListView {
        id: albumButtonList
        orientation: ListView.Horizontal
        width: parent.width - 60
        currentIndex: albumPage.keyList.indexOf(albumPage.key)
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

    GridView {
        id: albumShow
        width: parent.width
        height: parent.height - y
        anchors.top: albumButtonList.bottom
        anchors.margins: 30
        cellWidth: CoreData.cellItemWidth
        cellHeight: CoreData.cellItemWidth
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel {
            id: albumModel
        }

        delegate: CoreAlbumButton {
            y: 10
            x: 10
            id: albumButton
            albumId: inAlbumId
        }

    }


    function build () {
        albumModel.clear()
        const list = SQLData.getAlbumKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            albumButtonModel.append({keyString: list[i]})
        }
    }

    onKeyChanged: {
        albumModel.clear()
        const list = SQLData.getAlbum(key);
        for (let i=0; i<list.length; i++) {
            albumModel.append({inAlbumId: list[i]})
        }
    }
}


