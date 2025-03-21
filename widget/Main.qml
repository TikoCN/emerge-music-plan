import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import TikoAPI
import Tiko
import Widget

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
        var mousePos = centerView.mapToGlobal(0, 0)
        Setting.windowRect.width = window.width
        Setting.windowRect.height = window.height
        Setting.windowRect.x = mousePos.x
        Setting.windowRect.y = mousePos.y

        Setting.writeData()
        Core.writeJsonData()
    }

    StackView{
        id: centerView
        anchors.fill: parent

        PageMain{
            id:mainPage
            visible: false
        }

        Item{
            id: editPage
            visible: false

            //圆角背景
            Rectangle{
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
                anchors.top: mainView.bottom
            }
        }

        initialItem: editPage
    }

    ViewPlayingTable {
        id: playingTable
        width: parent.width * 2 / 3
        height: parent.height - bottomView.height
        y:10
    }

    //对话框
    TikoPopupMessage{
        id: popupMessage
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
    function stackMain(){
        if(MediaPlayer.playingMusic === null){
            TikoSeit.sendMessage(this, qsTr("请先播放音乐"), 1)
            return
        }

        if(centerView.currentItem != mainPage){
            centerView.pop(null)
            centerView.pushItem(mainPage)
            mainPage.actionStart()
        }
    }

    //切换到编辑页
    function stackCenter(){
        if(centerView.currentItem != editPage){
            mainPage.actionEnd()
            centerView.pop(null)
            centerView.pushItem(editPage)
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
            for (var i=0; i < Core.tableList.length; ++i) {
                mainView.addPlayTablePage(i)
                barView.addTable(i)
            }
        }
    }

    //关联
    Connections{
        target: Base

        function onSendMessage(text, type){
            TikoSeit.sendMessage(window, text, type)
        }
    }

    //清理数据
    function clearData(){
        barView.clearData()
        playingTable.clearData();
        mainView.clearData()
    }
}
