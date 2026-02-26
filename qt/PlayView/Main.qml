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
        anchors.fill: parent
        anchors.margins: TikoSeit.subitemSpace

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: "#80000000"
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }

        //圆角背景
        Rectangle{
            id: editPageBack
            anchors.fill: editPage
            topLeftRadius: 10
            topRightRadius: 10
            color: TikoSeit.theme.baseTheme.backgroundNormal
            y: showType === 0 ? 0 : -height
            z: -1

            Rectangle{
                anchors.fill: parent
                color: Qt.rgba(0,0,0,0)
                border.color: TikoSeit.theme.baseTheme.backgroundTransition
                topLeftRadius: 10
                topRightRadius: 10
                border.width: 0.5
                opacity: 0.7
            }
        }

        //中间内容导航
        ViewMain{
            id: mainView
            width: parent.width - barView.width - 20
            height: barView.height
            anchors.left: barView.right
            y: 10
        }

        ViewLeftBar{
            id: barView
            height: parent.height - bottomView.height - 10
            width: 300
            x: 10
            y: 10
        }

        //底部导航
        ViewBottomBar{
            id: bottomView
            height: 90
            width: parent.width
            y: 10 + barView.height
        }

        PageMusicPlay {
            id: musicPlayPage
            width: parent.width
            height: parent.height
            y: parent.height
            opacity: 0
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
                from: 10
                to: -mainView.height
                duration: editPage.duration
                easing.type: editPage.outType
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: 10 + barView.height
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
                from: -editPage.height
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
                to: 10
                duration: editPage.duration
                easing.type: editPage.inType
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: editPage.height
                to: 10 + barView.height
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
            width: parent.width * 2 / 3
            height: parent.height - bottomView.height
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
        // musicPlayPage.actionEnd()
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

    Component.onCompleted: {
        TikoSeit.buildTheme()
        load()
        MediaPlayer.initData()

        CoreData.windows = window
        CoreData.clearData.connect(clearData)
        CoreData.sendErrorMsg.connect(sendErroMsg)

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
        CoreData.playlist = JSON.parse(SQLData.getAllList())
        mainView.buildData()
    }

    Component {
        id: errorMsgCom
        TikoMessageLittle {
            type: 1
            anchors.centerIn: Overlay.overlay
        }
    }

    function sendErroMsg(msg) {
        if (errorMsgCom.status === Component.Ready) {
            const view = errorMsgCom.createObject(window, {message:msg});
            view.open()
        }
    }
}
