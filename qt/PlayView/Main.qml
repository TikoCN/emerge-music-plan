import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI
import Tiko
import PlayView
import QtQuick.Effects

TikoFrameless{
    id: window
    minimumWidth: 1000
    minimumHeight: 600
    x:Setting.windowRect.x
    y:Setting.windowRect.y
    width:Setting.windowRect.width
    height:Setting.windowRect.height
    visible: true
    title: qsTr("尘星音乐")
    onClosing: function(close) {
        close.accepted = false
        TaskbarManager.hideMainWindow()
    }
    property int showType: 0
    property bool isMaxSize: visibility === 4
    property var resourcePreviewWindow: null

    Component.onDestruction:{
        TaskbarManager.setMainWindow(null)
        //写入配置
        var mousePos = editPage.mapToGlobal(0, 0)
        Setting.windowRect.width = window.width
        Setting.windowRect.height = window.height
        Setting.windowRect.x = mousePos.x
        Setting.windowRect.y = mousePos.y

        Setting.writeData()
    }

    // 背景与阴影实现
    Item{
        id: editPage
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: space
        clip: true

        property int space: isMaxSize ? 0 : 10

        PageMusicPlay {
            id: musicPlayPage
            x: 0
            y: showType === 1 ? 0 : -height
            width: parent.width
            height: parent.height
            z: 1

            Behavior on y {
                NumberAnimation {
                    duration: editPage.duration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        //最底层圆角背景
        Rectangle{
            id: editPageBack
            anchors.fill: parent
            topLeftRadius: editPage.space
            topRightRadius: editPage.space
            color: TikoSeit.theme.colorMaxBottom
            y: showType === 0 ? 0 : -height
            z: -1

            Behavior on y {
                NumberAnimation {
                    duration: editPage.duration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        ViewLeftBar{
            id: barView
            height: parent.height
            width: 250
            x: showType === 0 ? 0 : -width

            Behavior on x {
                NumberAnimation {
                    duration: editPage.duration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        //中间内容导航
        ViewMain{
            id: mainView
            height: parent.height
            width: parent.width - barView.width - 10
            x: showType === 0 ? barView.width + 10 : parent.width

            Behavior on x {
                NumberAnimation {
                    duration: editPage.duration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        property int duration: 500
        property int inType: Easing.InOutQuad
        property int outType: Easing.InOutQuad

        //正在播放列表
        ViewPlayingList {
            id: playingPlaylist
            width: 400
            height: mainView.height
            y:10
        }
    }


    //切换到主页
    function stackMusicPaly(){
        if(MediaPlayer.playingMusic === null){
            TikoSeit.sendMessage(this, qsTr("请先播放音乐"), 1)
            return
        }

        musicPlayPage.setActive(true)
        showType = 1
    }

    //切换到编辑页
    function stackCenter(){
    }

    //清理数据
    function clearData(){
        playingPlaylist.clearData()
        mainView.stackDetail()
    }

    // 程序开始
    Component.onCompleted: {
        MediaPlayer.initData()

        CoreData.windows = window
        CoreData.clearData.connect(clearData)

        CoreData.windowShowMin.connect(showMinimized)
        CoreData.windowShowMax.connect(function () {
            if (window.visibility === 4) {
                window.showNormal()
            } else {
                window.showMaximized()
            }
        })
        CoreData.windowClose.connect(TaskbarManager.hideMainWindow)
        TaskbarManager.setMainWindow(window)
    }

    Connections {
        target: OnLine
        function onResourcePreviewReady(oldResourceUrl, newResourceData, filePath, isImage) {
            window.openResourcePreview({
                oldResourceUrl: oldResourceUrl,
                newResourceData: newResourceData,
                filePath: filePath,
                isImage: isImage
            })
        }

        function onResourceDownloadFailed(message) {
            window.showDownloadFailure(message)
        }
    }

    function openResourcePreview(properties) {
        if (resourcePreviewWindow !== null) {
            resourcePreviewWindow.destroy()
            resourcePreviewWindow = null
        }

        const component = Qt.createComponent("qrc:/qt/qml/PlayView/ResourcePreview.qml")
        if (component.status === Component.Ready) {
            createResourcePreviewWindow(component, properties)
        } else if (component.status === Component.Loading) {
            component.statusChanged.connect(function() {
                if (component.status === Component.Ready) {
                    window.createResourcePreviewWindow(component, properties)
                }
            })
        }
    }

    function createResourcePreviewWindow(component, properties) {
        const previewProperties = Object.assign({}, properties, {
            x: window.x + (window.width - 900) / 2,
            y: window.y + (window.height - 600) / 2
        })
        const previewWindow = component.createObject(null, previewProperties)
        if (previewWindow === null) {
            return
        }

        resourcePreviewWindow = previewWindow
        previewWindow.dismissed.connect(function() {
            if (window.resourcePreviewWindow === previewWindow) {
                window.resourcePreviewWindow = null
            }
        })
        previewWindow.show()
    }

    function showDownloadFailure(message) {
        const component = Qt.createComponent("qrc:/qt/qml/PlayView/ResourceDownloadFailed.qml")
        if (component.status === Component.Ready) {
            const failureWindow = component.createObject(null, {
                message: message,
                transientParent: window,
                x: window.x + (window.width - 420) / 2,
                y: window.y + (window.height - 220) / 2
            })
            if (failureWindow !== null) {
                failureWindow.show()
            }
        }
    }
}
