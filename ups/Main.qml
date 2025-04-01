import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MediaerAPI
import Tiko
import Ups


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

    Component.onDestruction:{
        //写入配置
        var mousePos = editPage.mapToGlobal(0, 0)
        Setting.windowRect.width = window.width
        Setting.windowRect.height = window.height
        Setting.windowRect.x = mousePos.x
        Setting.windowRect.y = mousePos.y

        Setting.writeData()
        Core.writeJsonData()
    }

    PageMusicPlay {
        id: musicPlayPage
        width: parent.width
        height: parent.height
        y: parent.height
    }

    Item{
        id: editPage
        width: parent.width
        height: parent.height

        //圆角背景
        Rectangle{
            id: editPageBack
            width: parent.width
            height: parent.height
            topLeftRadius: 10
            topRightRadius: 10
            color: Setting.backdropColor

            Rectangle{
                anchors.fill: parent
                border.color: Setting.transparentColor
                topLeftRadius: 10
                topRightRadius: 10
                border.width: 0.5
                opacity: 0.3
            }
        }

        ViewLeftBar{
            id: barView
            height: parent.height - bottomView.height - 10
            width: 200
            x: 10
            y: 10
        }

        //中间内容导航
        ViewMain{
            id: mainView
            width: parent.width - barView.width - 20
            height: barView.height
            anchors.left: barView.right
            y: 10
        }

        //底部导航
        ViewBottomBar{
            id: bottomView
            height: 90
            width: parent.width
            y: 10 + barView.height
        }
    }

    SequentialAnimation {
        id: trunToMusicPlayAnimation

        ParallelAnimation{
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: 10
                to: -mainView.height
                duration: 500
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: 10 + barView.height
                to: editPage.height
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }

        ParallelAnimation {
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: 0
                to: -editPage.height
                duration: 500
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: musicPlayPage.height
                to: 0
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    }

    SequentialAnimation {
        id: trunToMainAnimation

        ParallelAnimation {
            NumberAnimation {
                target: editPageBack
                property: "y"
                from: -editPage.height
                to: 0
                duration: 500
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: musicPlayPage
                property: "y"
                from: 0
                to: musicPlayPage.height
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }

        ParallelAnimation{
            NumberAnimation {
                targets: [mainView, barView]
                property: "y"
                from: -mainView.height
                to: 10
                duration: 500
                easing.type: Easing.InOutQuad
            }

            NumberAnimation {
                target: bottomView
                property: "y"
                from: editPage.height
                to: 10 + barView.height
                duration: 500
                easing.type: Easing.InOutQuad
            }
        }
    }

    ViewPlayingTable {
        id: playingTable
        width: parent.width * 2 / 3
        height: parent.height - bottomView.height
        y:10
    }

    //计算长度
    TextMetrics{
        text: qsTr("00:00")
        font: Setting.mainFont
        onBoundingRectChanged: {
            CoreData.timeWidth = boundingRect.width
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
        barView.clearData()
        playingTable.clearData();
        mainView.clearData()
    }

    //关联
    Connections{
        target: Base

        function onMessage(text, type){
            TikoSeit.sendMessage(window, text, type)
        }
    }

    //关联
    Connections{
        target: Core

        function onTableAdd(){
            mainView.addPlayTablePage(Core.tableList.length - 1)
            barView.addTable(Core.tableList.length - 1)
        }

        function onFinishInit(){
            mainView.buildData()
            for (var i=0; i < Core.tableList.length; ++i) {
                mainView.addPlayTablePage(i)
                barView.addTable(i)
            }
        }
    }
}
