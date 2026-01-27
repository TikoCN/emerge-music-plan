import QtQuick
import Tiko
import MediaerAPI
import PlayView

Item {
    width: parent.width
    height: childrenRect.height
    property string artistName: ""

    TikoTextLine {
        id: artistNameLine
        text: artistName
    }

    GridView {
        id: artistShow
        width: parent.width
        height: 100
        anchors.top: artistNameLine.bottom
        anchors.margins: CoreData.cellItemSpace
        cellWidth: CoreData.cellItemWidth
        cellHeight: CoreData.cellItemWidth
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: 0
        preferredHighlightEnd: 0

        model: ListModel {
            id: artistModel
        }

        delegate: CoreArtistButton {
            id: artistButton
            artistId: inArtistId
        }
    }

    onArtistNameChanged: {
        artistModel.clear()
        const list = SQLData.getArtist(artistName);
        for (let i=0; i<list.length; i++) {
            artistModel.append({inArtistId: list[i]})
        }
        updataHeight()
    }

    onWidthChanged: Qt.callLater(updataHeight)

    function updataHeight() {
        if (artistShow.width === 0 || CoreData.cellItemWidth === 0) return

                // 计算每行能显示多少个专辑
                let columns = Math.floor(artistShow.width / CoreData.cellItemWidth)
                if (columns === 0) columns = 1 // 至少显示一个

                // 计算需要多少行
                let rows = Math.ceil(artistModel.count / columns)

                // 设置GridView高度
                artistShow.height = rows * (CoreData.cellItemHeight + CoreData.cellItemSpace)
    }
}
