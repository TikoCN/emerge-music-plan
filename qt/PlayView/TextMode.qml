import QtQuick.Controls
import QtQml
import MediaerAPI
import QtQuick
import Tiko
import PlayView

Window {
    id: textModeWindow
    width: 600
    height: 400

    Item {
        anchors.fill: parent

        Row {
            id: toolRow
            TikoButtonNormal {
                width: 150
                text: "随机插入300 music"
                onClick: musicListView.insertMusicList(SQLData.getMusicRandList(300))
            }

            TikoButtonNormal {
                width: 150
                text: "随机获取300 SQL MusicId"
                onClick: SQLData.getMusicRandList(300)
            }
        }

        MusicListView {
            id: musicListView
            anchors.top: toolRow.bottom
            anchors.bottom: parent.bottom
            width: parent.width
        }
    }
}
