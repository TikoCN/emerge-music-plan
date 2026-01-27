import QtQuick
import MediaerAPI
import Tiko
import PlayView

Item {
    id: playerLrcPlayList

    property bool have: false

    //插入歌词
    function buildLrcList() {
        const length = MediaPlayer.getLrcListLength();
        for (let i = 0; i < length; i++) {
            lrcDataList.append({
                lrc: i
            });
        }
    }

    Component.onCompleted: buildLrcList()

    MouseArea {
        id: mouseHover

        acceptedButtons: Qt.RightButton | Qt.LeftButton
        anchors.fill: parent
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

                    onClicked: Setting.mainLrcFont.pixelSize++
                }
                TikoMenuItem {
                    icon.source: "qrc:/image/size+.png"
                    text: qsTr("字体减小")

                    onClicked: Setting.mainLrcFont.pixelSize--
                }
                TikoMenuSpeacer {
                }
                TikoMenuItem {
                    text: qsTr("打开歌词")

                    onClicked: MediaPlayer.playingMusic.openMusicLrc()
                }
                TikoMenuItem {
                    text: qsTr("编辑歌词")

                    onClicked: CoreData.editMusic(playerLrcPlayList, MediaPlayer.playingMusic)
                }
            }
        }
    }
    ListView {
        id: lrcList
        anchors.fill: parent
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: width / 2
        preferredHighlightEnd: width / 2

        delegate: CoreLrcLine {
            lrcId: lrc
            width: lrcList.width
        }
        model: ListModel {
            id: lrcDataList

        }
    }

    //关联
    Connections {
        function onLrcLoaded() {
            playerLrcPlayList.buildLrcList();
        }
        function onPlayingLrcIdChange(playingLrcId) {
            if (!show && !mouseHover.containsMouse) {
                return;
            }
            lrcList.currentIndex = playingLrcId;
        }

        target: MediaPlayer
    }
}
