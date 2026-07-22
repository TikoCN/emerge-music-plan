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
                       menu.popup();
                   }
               }

    Component {
        id: comMenu

        TikoMenu {
            id: menu

            onClosed: menu.destroy()

            TikoMenuItem {
                icon.source: "qrc:/image/size+.png"
                text: qsTr("字体加大")
                onClicked: Setting.lrcFont.pointSize++
            }
            TikoMenuItem {
                icon.source: "qrc:/image/size+.png"
                text: qsTr("字体减小")
                onClicked: Setting.lrcFont.pointSize--
            }
            TikoMenuSpeacer {}
            TikoMenuItem {
                text: qsTr("编辑歌词")
                onClicked: CoreData.editMusic(playerLrcPlayList, MediaPlayer.playingMusic)
            }
        }
    }

    ListView {
        id: lrcList
        anchors.fill: parent
        highlightRangeMode: ListView.StrictlyEnforceRange
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
    }


    //关联
    Connections {
        target: MediaPlayer

        function onLrcLoaded() {
            buildLrcList();
        }
        function onPlayingLrcIdChanged(playingLrcId) {
            lrcList.currentIndex = playingLrcId;
        }
    }

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
