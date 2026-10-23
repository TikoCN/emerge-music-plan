import QtQuick
import QtQuick.Window
import QtQuick.Controls.Basic
import MediaerAPI
import Tiko
import PlayView

TikoFrameless {
    id: toolEditMusic
    title: qsTr("编辑音乐信息")
    width: 800
    height: 600
    minimumWidth: 640
    minimumHeight: 420
    visible: false
    flags: Qt.Window | Qt.FramelessWindowHint

    property int musicId: -1

    Item {
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            color: TikoSeit.theme.colorBgView
            radius: 10
        }

        TikoWindow {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            title: toolEditMusic.title
            windowTarget: toolEditMusic
            onCloseRequested: toolEditMusic.close()
        }

        Column {
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: TikoSeit.emphasizeMargins
            spacing: 12

            Row {
                width: parent.width
                spacing: 8

                TikoButtonDefault {
                    textLine.text: qsTr("编辑音乐")
                    icon.source: "qrc:/image/editMusic.svg"
                    checkable: true
                    checked: stackView.currentItem !== null
                             && stackView.currentItem.objectName === "musicEditorPage"
                    onLeftClicked: toolEditMusic.stackEditMusicPage()
                }

                TikoButtonDefault {
                    textLine.text: qsTr("编辑歌词")
                    icon.source: "qrc:/image/editLrc.svg"
                    checkable: true
                    checked: stackView.currentItem !== null
                             && stackView.currentItem.objectName === "lyricEditorPage"
                    onLeftClicked: toolEditMusic.stackEditLrcPage()
                }
            }

            StackView {
                id: stackView
                width: parent.width
                height: parent.height - 60
                clip: true

                initialItem: EditPageMusic {
                    objectName: "musicEditorPage"
                }
            }
        }
    }

    Component {
        id: lrcEditPageComponent

        EditPageLrc {
            objectName: "lyricEditorPage"
        }
    }

    Component {
        id: musicEditPageComponent

        EditPageMusic {
            objectName: "musicEditorPage"
        }
    }

    /**
     * @brief 切换到音乐信息编辑页。
     */
    function stackEditMusicPage() {
        if (stackView.currentItem === null || stackView.currentItem.objectName !== "musicEditorPage") {
            const page = stackView.replace(musicEditPageComponent)
            if (page !== null)
                page.init(toolEditMusic.musicId)
        }
    }

    /**
     * @brief 切换到歌词编辑页。
     */
    function stackEditLrcPage() {
        if (stackView.currentItem === null || stackView.currentItem.objectName !== "lyricEditorPage") {
            const page = stackView.replace(lrcEditPageComponent)
            if (page !== null)
                page.init(toolEditMusic.musicId)
        }
    }

    /**
     * @brief 初始化指定歌曲的音乐信息和歌词编辑页面。
     * @param musicId 歌曲 ID。
     */
    function build(musicId) {
        toolEditMusic.musicId = musicId
        stackView.currentItem.init(musicId)
        stackEditMusicPage()
    }
}
