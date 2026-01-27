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
        currentIndex: 0
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

        onCurrentIndexChanged: {
            if (artistNameList.currentIndex !== currentIndex) {
                artistNameList.currentIndex = currentIndex
            }
        }
    }

    ListView {
        id: artistNameList
        width: artistButtonList.width
        anchors.top: artistButtonList.bottom
        anchors.margins: 30
        anchors.bottom: parent.bottom
        clip: true
        currentIndex: artistButtonList.currentIndex
        highlightMoveDuration: 1000
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        flickDeceleration: 100

        model: ListModel {
            id: artistNameModel
        }

        delegate: CoreArtistNameGrid {
            x: 10
            width: artistNameList.width - 20
            id: artistButton
            artistName: keyString
        }

        onCurrentIndexChanged: {
            if (artistButtonList.currentIndex !== currentIndex) {
                artistButtonList.currentIndex = currentIndex
            }
        }
    }


    function build () {
        const list = SQLData.getArtistKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            artistButtonModel.append({keyString: list[i]})
            artistNameModel.append({keyString: list[i]})
        }
    }

    onKeyChanged: {
        artistButtonList.currentIndex = keyList.indexOf(key)
    }
}
