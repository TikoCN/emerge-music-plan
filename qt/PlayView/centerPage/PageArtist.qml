import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import PlayView
import QtQuick.Layouts

Item {
    id: artistPage

    property string key: ""
    property var keyList: []
    property var keyInGirdIdList: []
    property int nameKeyId: -1
    property int artistStartPos: 0
    property int loadNumber: 0
    property bool isLoading: false

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
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4
        snapMode: ListView.SnapToItem

        model: ListModel {
            id: artistButtonModel
        }

        delegate: TikoTextLine {
            text: keyString
            height: 40
            opacity: ListView.isCurrentItem ? 1 : 0.3

            MouseArea {
                anchors.fill: parent
                onClicked: key = keyString
            }
        }
    }

    GridView {
        id: artistCoreList
        width: artistButtonList.width
        anchors.top: artistButtonList.bottom
        anchors.margins: 30
        anchors.bottom: parent.bottom
        clip: true
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0
        flickDeceleration: 100
        reuseItems: true
        cellWidth: CoreData.cellItemWidth
        cellHeight: CoreData.cellItemHeight

        model: ListModel {
            id: artistCoreModel
        }

        delegate: Loader {
            sourceComponent: model.isTextLine ? textLineCom : artistCellCom

            Component {
                id: textLineCom
                TikoTextLine {
                    width: CoreData.cellItemWidth
                    height: CoreData.cellItemHeight
                    text: model.name
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Component {
                id: artistCellCom
                CoreArtistButton {
                    artistId: model.artistId
                }
            }
        }

        onFlickEnded: {
            if (contentY + height >= contentHeight - 100) {
                loadMoreData();
            }
        }
    }


    function build () {
        artistButtonModel.clear()
        artistCoreModel.clear()

        const list = SQLData.getArtistKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            artistButtonModel.append({keyString: list[i]})
        }

        loadMoreData()
    }

    function loadMoreData() {
        if(isLoading) return
        isLoading = true
        const loadAim = 100
        loadNumber = 0

        while(loadNumber <= loadAim){
            let isCoreOver = loadMoreCore()
            if (isCoreOver) {
                let isNameKeyOver = loadNewNameKey()
                if (isNameKeyOver) {
                    break
                }
            }
        }

        isLoading = false
    }

    function loadMoreCore() {
        let size = 50
        const nameKey = keyList[nameKeyId]
        let list = SQLData.getArtistIdByNameKey(nameKey, size, artistStartPos)

        if (list.length === 0) return true

        list.forEach(artistId =>{
            artistCoreModel.append({isTextLine: false, artistId:artistId})
        })

        artistStartPos += list.length
        loadNumber += list.length
        return false
    }

    function loadNewNameKey() {
        if (nameKeyId === keyList.length - 1) return true
        nameKeyId++
        artistStartPos = 0

        const nameKey = keyList[nameKeyId]
        artistCoreModel.append({isTextLine: true, name:nameKey})
        keyInGirdIdList.push(artistCoreList.count)

        loadNumber++
        return false
    }

    onKeyChanged: {
        let index = keyList.indexOf(key)
        artistButtonList.currentIndex = index
        if(index < keyInGirdIdList.length && index >= 0) artistCoreList.currentIndex = keyInGirdIdList[index]
    }
}
