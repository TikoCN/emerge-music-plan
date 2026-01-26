import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import PlayView

Item {
    id: artistPage

    property string key: ""
    property string keyList: []

    // 跳转按钮列表
    ListView {
        id: artistButtonList
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
            id: artistButtonModel
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
        id: artistShow
        width: parent.width
        height: parent.height - y
        anchors.top: artistButtonList.bottom
        anchors.margins: 30
        cellWidth: CoreData.cellItemWidth
        cellHeight: CoreData.cellItemWidth
        clip: true
        currentIndex: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel {
            id: artistModel
        }

        delegate: CoreArtistButton {
            y: 10
            x: 10
            id: artistButton
            artistId: inArtistId
        }
    }


    function build () {
        artistModel.clear()
        const list = SQLData.getArtistKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            artistButtonModel.append({keyString: list[i]})
        }
    }

    onKeyChanged: {
        artistModel.clear()
        const list = SQLData.getArtist(key);
        for (let i=0; i<list.length; i++) {
            artistModel.append({inArtistId: list[i]})
        }
    }
}

