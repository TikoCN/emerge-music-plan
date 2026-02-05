import QtQuick
import QtQuick.Controls
import Tiko
import MediaerAPI
import PlayView
import QtQuick.Layouts

Item {
    id: albumPage

    property string key: ""
    property var keyList: []
    property var keyInGirdIdList: []
    property int nameKeyId: -1
    property int albumStartPos: 0
    property int loadNumber: 0
    property bool isLoading: false

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
        highlightRangeMode: ListView.ApplyRange
        preferredHighlightBegin: width / 4
        preferredHighlightEnd: width / 4
        snapMode: ListView.SnapToItem

        model: ListModel {
            id: albumButtonModel
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
        id: albumCoreList
        width: albumButtonList.width
        anchors.top: albumButtonList.bottom
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
            id: albumCoreModel
        }

        delegate: Loader {
            sourceComponent: model.isTextLine ? textLineCom : albumCellCom

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
                id: albumCellCom
                CoreAlbumButton {
                    albumId: model.albumId
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
        albumButtonModel.clear()
        albumCoreModel.clear()

        const list = SQLData.getAlbumKeyList();
        if (list.length > 0) {
            keyList = list
            key = list[0]
        }

        for (let i=0; i<list.length; i++) {
            albumButtonModel.append({keyString: list[i]})
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
        let list = SQLData.getAlbumIdByNameKey(nameKey, size, albumStartPos)

        if (list.length === 0) return true

        list.forEach(albumId =>{
            albumCoreModel.append({isTextLine: false, albumId:albumId})
        })

        albumStartPos += list.length
        loadNumber += list.length
        return false
    }

    function loadNewNameKey() {
        if (nameKeyId === keyList.length - 1) return true
        nameKeyId++
        albumStartPos = 0

        const nameKey = keyList[nameKeyId]
        albumCoreModel.append({isTextLine: true, name:nameKey})
        keyInGirdIdList.push(albumCoreList.count)

        loadNumber++
        return false
    }

    onKeyChanged: {
        let index = keyList.indexOf(key)
        albumButtonList.currentIndex = index
        if(index < keyInGirdIdList.length && index >= 0) albumCoreList.currentIndex = keyInGirdIdList[index]
    }
}


