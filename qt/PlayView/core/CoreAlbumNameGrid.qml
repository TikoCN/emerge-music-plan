import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    width: parent.width
    height: childrenRect.height
    property string albumName: ""

    TikoTextLine {
        id: albumNameLine
        text: albumName
    }

    GridView {
        id: albumShow
        width: parent.width
        height: 100
        anchors.top: albumNameLine.bottom
        anchors.margins: CoreData.cellItemSpace
        cellWidth: CoreData.cellItemWidth
        cellHeight: CoreData.cellItemWidth
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel {
            id: albumModel
        }

        delegate: CoreAlbumButton {
            id: albumButton
            albumId: inAlbumId
        }
    }

    onAlbumNameChanged: {
        albumModel.clear()
        const list = SQLData.getAlbum(albumName);
        for (let i=0; i<list.length; i++) {
            albumModel.append({inAlbumId: list[i]})
        }
        updataHeight()
    }

    onWidthChanged: Qt.callLater(updataHeight)

    function updataHeight() {
        if (albumShow.width === 0 || CoreData.cellItemWidth === 0) return

                // 计算每行能显示多少个专辑
                let columns = Math.floor(albumShow.width / CoreData.cellItemWidth)
                if (columns === 0) columns = 1 // 至少显示一个

                // 计算需要多少行
                let rows = Math.ceil(albumModel.count / columns)

                // 设置GridView高度
                albumShow.height = rows * (CoreData.cellItemHeight + CoreData.cellItemSpace)
    }
}
