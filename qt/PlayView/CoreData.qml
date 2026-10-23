pragma Singleton
import QtQuick
import QtQuick.Window
import PlayView
import MediaerAPI
import Tiko
import DataType

QtObject {
    property var playlist: []

    property int pageSize: 30
    property TikoFrameless windows

    signal stackPlaylist(int page)
    signal stackSeit()
    signal stackRcommend()
    signal stackLibrary()
    signal stackAlbum(int album)
    signal stackArtist(int artist)
    signal stackDetail()
    signal fixedPageChanged()
    property int fixedPageIndex: -1
    signal sendMsg(string msg, int type)
    signal clearData()
    signal sendErrorMsg(string msg)

    signal windowShowMin()
    signal windowShowMax()
    signal windowClose()

    signal autoUpdateUI()

    function editMusic(parent, musicId) {
        const component = Qt.createComponent("qrc:/qt/qml/PlayView/ToolEditMusicPage/ToolEditMusic.qml")
        const ownerWindow = windows
        if (ownerWindow === null) {
            console.log("无法确定编辑窗口的所属主窗口")
            return
        }

        if (component.status === Component.Loading) {
            component.statusChanged.connect(function() {
                if (component.status === Component.Ready)
                    createMusicEditWindow(component, ownerWindow, musicId)
                else if (component.status === Component.Error)
                    console.log(component.errorString())
            })
            return
        }

        if (component.status !== Component.Ready) {
            console.log(component.errorString())
            return
        }

        createMusicEditWindow(component, ownerWindow, musicId)
    }

    function createMusicEditWindow(component, ownerWindow, musicId) {
        const item = component.createObject(null, {
            transientParent: ownerWindow,
            x: ownerWindow.x + (ownerWindow.width - 800) / 2,
            y: ownerWindow.y + (ownerWindow.height - 600) / 2
        })
        if (item === null) {
            console.log(component.errorString())
            return
        }

        item.show()
        item.requestActivate()
        item.build(musicId)
    }

    property Timer internalTimer: Timer {
        interval: 20
        running: true
        repeat: true
        onTriggered: autoUpdateUI()
    }
}
