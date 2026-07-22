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
    width:Setting.windowRect.width
    minimumWidth: 1000
    height:Setting.windowRect.height
    minimumHeight: 600
    x:Setting.windowRect.x
    y:Setting.windowRect.y
    visible: true
    title: qsTr("尘星音乐")
    property int showType: 0
    property bool isMaxSize: visibility === 4

    Component.onDestruction:{
        //写入配置
        var mousePos = editPage.mapToGlobal(0, 0)
        Setting.windowRect.width = window.width
        Setting.windowRect.height = window.height
        Setting.windowRect.x = mousePos.x
        Setting.windowRect.y = mousePos.y

        Setting.writeData()
    }

    Item{
        id: editPage
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: space

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: TikoSeit.theme.colorMaxTop
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }

        property int space: isMaxSize ? 0 : 10

        PageMusicPlay {
            id: musicPlayPage
            anchors.fill: parent
            opacity: 0
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
        }

        //中间内容导航
        ViewMain{
            id: mainView
            anchors.top: parent.top
            anchors.left: barView.right
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: TikoSeit.subitemSpace
        }

        ViewLeftBar{
            id: barView
            height: parent.height
            width: 250
        }

        //底部导航
        ViewBottomBar{
            id: bottomView
            height: 90
            anchors.bottom: parent.bottom
            anchors.margins: 30
            anchors.left: barView.right
            anchors.right: parent.right
        }

        property int duration: 500
        property int inType: Easing.InOutQuad
        property int outType: Easing.InOutQuad
        // 切换到音乐播放界面动画
        ParallelAnimation {
            id: trunToMusicPlayAnimation
            onFinished: showType = 1

            // 出
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: 0
                to: -mainView.height
                duration: editPage.duration
                easing.type: editPage.outType
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: barView.height
                to: editPage.height
                duration: editPage.duration
                easing.type: editPage.outType
            }

            NumberAnimation {
                targets: [mainView, barView, bottomView]
                property: "opacity"
                from: 1
                to: 0
                duration: editPage.duration
                easing.type: editPage.outType
            }

            // 进
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: 0
                to: -editPage.height
                duration: editPage.duration
                easing.type: editPage.inType
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: musicPlayPage.height
                to: 0
                duration: editPage.duration
                easing.type: editPage.inType
            }

            NumberAnimation {
                target: musicPlayPage
                property: "opacity"
                from: 0
                to: 1
                duration: editPage.duration
                easing.type: editPage.inType
            }

        }

        ParallelAnimation {
            id: trunToMainAnimation
            onFinished: {
                musicPlayPage.actionEnd()
                showType = 0
            }


            // 出
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: -height
                to: 0
                duration: editPage.duration
                easing.type: editPage.outType
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: 0
                to: musicPlayPage.height
                duration: editPage.duration
                easing.type: editPage.outType
            }

            NumberAnimation {
                target: musicPlayPage
                property: "opacity"
                from: 1
                to: 0
                duration: editPage.duration
                easing.type: editPage.outType
            }

            // 进
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: -mainView.height
                to: 0
                duration: editPage.duration
                easing.type: editPage.inType
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: editPage.height
                to: barView.height
                duration: editPage.duration
                easing.type: editPage.inType
            }

            NumberAnimation {
                targets: [mainView, barView, bottomView]
                property: "opacity"
                from: 0
                to: 1
                duration: editPage.duration
                easing.type: editPage.inType
            }
        }

        ViewPlayingList {
            id: playingPlayList
            width: parent.width * 0.3
            height: parent.height
            y:10
        }
    }


    //切换到主页
    function stackMusicPaly(){
        if(MediaPlayer.playingMusic === null){
            TikoSeit.sendMessage(this, qsTr("请先播放音乐"), 1)
            return
        }

        musicPlayPage.actionStart()
        trunToMusicPlayAnimation.start()
    }

    //切换到编辑页
    function stackCenter(){
        // 移动到动画结束
        musicPlayPage.actionEnd()
        trunToMainAnimation.start()
    }

    //清理数据
    function clearData(){
        playingPlayList.clearData()
        mainView.turnToSeit()
    }

    Connections{
        target: DataActive
        function onFinish(){
            load()
        }
    }

    // 程序开始
    Component.onCompleted: {
        load()
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
        CoreData.windowClose.connect(close)
    }

    function load() {
        CoreData.playlist = JSON.parse(PlayListLibrary.getAllList())
        mainView.buildData()
    }
}
