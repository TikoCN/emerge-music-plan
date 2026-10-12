import QtQuick
import MediaerAPI
import Tiko
import PlayView

MouseArea {
    id: mouseHover
    property bool have: false

    acceptedButtons: Qt.RightButton | Qt.LeftButton
    hoverEnabled: true

    onClicked: mouse => {
                   if (mouse.button === Qt.RightButton) {
                       const menu = comMenu.createObject(mouseHover);
                       menu.popupAt(mouseHover, mouse.x, mouse.y);
                   }
               }

    Component {
        id: comMenu

        TikoMenu {
            id: menu

            onClosed: menu.destroy()

            TikoMenuItem {
                icon.source: "qrc:/image/size+.svg"
                text: qsTr("字体加大")
                onClicked: Setting.lrcFont.pointSize++
            }
            TikoMenuItem {
                icon.source: "qrc:/image/size+.svg"
                text: qsTr("字体减小")
                onClicked: Setting.lrcFont.pointSize--
            }
            TikoMenuSpacer {}
            TikoMenuItem {
                text: qsTr("编辑歌词")
                onClicked: CoreData.editMusic(playerLrcPlaylist, MediaPlayer.playingMusic)
            }
        }
    }

    ListView {
        id: lrcList
        anchors.fill: parent
        property bool pointerHovered: false
        highlightRangeMode: ListView.NoHighlightRange
        preferredHighlightBegin: width / 2
        preferredHighlightEnd: width / 2
        reuseItems: true
        highlightMoveVelocity: -1
        highlightMoveDuration: 500

        delegate: CoreLrcLine {
            lrcId: model.lrc
            width: lrcList.width
        }
        model: ListModel {
            id: lrcDataList
        }

        Text {
            anchors.centerIn: parent
            visible: lrcDataList.count === 0
            text: "Music  Lryic"
            color: TikoSeit.theme.colorTextDefault
            font: Setting.lrcFont
        }
    }


    //关联
    Connections {
        target: MediaPlayer

        function onLrcLoaded() {
            buildLrcList();
        }
        function onPlayingLrcIdChanged(playingLrcId) {
            if (!lrcList.pointerHovered) {
                lrcList.currentIndex = playingLrcId;
            }
        }
    }

    onPositionChanged: mouse => {
        lrcList.pointerHovered = true;
    }

    onExited: lrcList.pointerHovered = false

    //插入歌词
    function buildLrcList() {
        lrcDataList.clear()
        const length = MediaPlayer.getLrcListLength();
        for (let i = 0; i < length; i++) {
            lrcDataList.append({
                                   lrc: i
                               });
        }
    }

    function clearData() {
        lrcDataList.clear()
    }
}
