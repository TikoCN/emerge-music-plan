import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI
import Tiko
import PlayView


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

    Binding{
        target: TikoSeit
        property: "themeColor"
        value: Setting.themeColor
    }
    Binding{
        target: TikoSeit
        property: "transparentColor"
        value: Setting.transparentColor
    }
    Binding{
        target: TikoSeit
        property: "backdropColor"
        value: Setting.backdropColor
    }
    Binding{
        target: TikoSeit
        property: "fontFamily"
        value: Setting.mainFont.family
    }
    Binding{
        target: TikoSeit
        property: "fontPixelSize"
        value: Setting.mainFont.pixelSize
    }
    Binding {
        target: CoreData
        property: "sizeChange"
        value: window.sizeChange
    }

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

        //圆角背景
        Rectangle{
            id: editPageBack
            width: parent.width
            height: parent.height
            topLeftRadius: 10
            topRightRadius: 10
            color: Setting.backdropColor
            y: showType === 0 ? 0 : -height

            Rectangle{
                anchors.fill: parent
                border.color: Setting.transparentColor
                topLeftRadius: 10
                topRightRadius: 10
                border.width: 0.5
                opacity: 0.3
            }
        }

        //中间内容导航
        ViewMain{
            id: mainView
            width: parent.width - barView.width - 20
            height: barView.height
            anchors.left: barView.right
            y: showType === 0 ? 10 : -height
        }

        ViewLeftBar{
            id: barView
            height: parent.height - bottomView.height - 10
            width: 200
            x: 10
            y: showType === 0 ? 10 : -height
        }

        //底部导航
        ViewBottomBar{
            id: bottomView
            height: 90
            width: parent.width
            y: showType === 0 ? 10 + barView.height : editPage.height
        }
    }

    PageMusicPlay {
        id: musicPlayPage
        width: parent.width
        height: parent.height
        y: showType === 1 ? 0 : parent.height
        clip: true
    }

    // 切换到音乐播放界面动画
    SequentialAnimation {
        id: trunToMusicPlayAnimation
        onFinished: showType = 1

        ParallelAnimation{
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: 10
                to: -mainView.height
                duration: 300
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: 10 + barView.height
                to: editPage.height
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        ParallelAnimation {
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: 0
                to: -editPage.height
                duration: 300
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: musicPlayPage.height
                to: 0
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    }

    SequentialAnimation {
        id: trunToMainAnimation
        onFinished: showType = 0

        ParallelAnimation {
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: -editPage.height
                to: 0
                duration: 300
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: 0
                to: musicPlayPage.height
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        ParallelAnimation{
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: -mainView.height
                to: 10
                duration: 300
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: editPage.height
                to: 10 + barView.height
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    }

    ViewPlayingList {
        id: playingPlayList
        width: parent.width * 2 / 3
        height: parent.height - bottomView.height
        y:10
    }

    //计算长度
    TextMetrics{
        text: qsTr("00:00")
        font: Setting.mainFont
        onBoundingRectChanged: {
            CoreData.timeWidth = boundingRect.width + 20
        }
    }
    TextMetrics{
        text: qsTr("00-00-00 00:00:0000")
        font: Setting.mainFont
        onBoundingRectChanged: {
            CoreData.editTimeWidth = boundingRect.width
        }
    }
    TextMetrics{
        text: qsTr("000000000")
        font: Setting.mainFont
        onBoundingRectChanged: {
            CoreData.playNumberWidth = boundingRect.width
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
        musicPlayPage.actionEnd()
        trunToMainAnimation.start()
    }

    //清理数据
    function clearData(){
        playingPlayList.clearData()
        mainView.turnToSeit()
    }

    //关联
    Connections{
        target: Base

        function onMessage(text, type){
            TikoSeit.sendMessage(window, text, type)
        }
    }

    Connections{
        target: Core
        function onFinish(){
<<<<<<< Updated upstream:PlayView/Main.qml
            CoreData.table = JSON.parse(SQLData.getAllList())
            mainView.buildData()
=======
            load()
        }
    }

    Component.onCompleted: {
        load()
        CoreData.clearData.connect(clearData)
        CoreData.sendErrorMsg.connect(sendErroMsg)
    }

    function load(){
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
            var view = errorMsgCom.createObject(window, {message:msg})
            view.open()
>>>>>>> Stashed changes:qt/PlayView/Main.qml
        }
    }
}
