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
        highlightRangeMode: ListView.StrictlyEnforceRange
        preferredHighlightBegin: height / 2
        preferredHighlightEnd: height / 2
        reuseItems: true
        highlightMoveVelocity: -1
        highlightMoveDuration: 500

        delegate: CoreLrcLine {
            lrcId: model.lrcId
            startTime: model.startTime
            duration: model.endTime
            startList: model.startList
            endList: model.endList
            textList: model.textList
            helpTextList: model.helpTextList
            width: lrcList.width
        }
        model: MediaPlayer.lrcModel

        Text {
            anchors.centerIn: parent
            visible: lrcList.count === 0
            text: "Music  Lryic"
            color: TikoSeit.theme.colorTextDefault
            font: Setting.lrcFont
        }
    }


    //关联
    Connections {
        target: MediaPlayer

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

    // 显示页面时用歌词列表模型恢复角色数据。
    function buildLrcList() {
        MediaPlayer.refreshLrcModel()
        if (MediaPlayer.playingLrcId >= 0 && MediaPlayer.playingLrcId < lrcList.count) {
            lrcList.currentIndex = MediaPlayer.playingLrcId
        }
    }

    function clearData() {
        MediaPlayer.clearLrcModel()
    }
}
