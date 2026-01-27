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
        currentIndex: 0
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

        onCurrentIndexChanged: {
            if (albumNameList.currentIndex !== currentIndex) {
                albumNameList.currentIndex = currentIndex
            }
        }
    }

    ListView {
        id: albumNameList
        width: albumButtonList.width
        anchors.top: albumButtonList.bottom
        anchors.margins: 30
        anchors.bottom: parent.bottom
        clip: true
        currentIndex: albumButtonList.currentIndex
        highlightMoveDuration: 1000
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        flickDeceleration: 100

        model: ListModel {
            id: albumNameModel
        }

        delegate: CoreAlbumNameGrid {
            x: 10
            width: albumNameList.width - 20
            id: albumButton
            albumName: keyString
        }

        onCurrentIndexChanged: {
            if (albumButtonList.currentIndex !== currentIndex) {
                albumButtonList.currentIndex = currentIndex
            }
        }
    }


    function build () {
        const list = SQLData.getAlbumKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            albumButtonModel.append({keyString: list[i]})
            albumNameModel.append({keyString: list[i]})
        }
    }

    onKeyChanged: {
        albumButtonList.currentIndex = keyList.indexOf(key)
    }
}


