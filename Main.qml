import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQml
import MyAPI

MyFrameless{
    id: window
    width:Setting.windowRect.width
    height:Setting.windowRect.height
    x:Setting.windowRect.x
    y:Setting.windowRect.y
    visible: true
    title: qsTr("尘星音乐")

    property var tableList: []//列表指针

    Component.onDestruction:{
        //写入配置
        var mousePos = centerView.mapToGlobal(0, 0)
        Setting.windowRect.width = window.width
        Setting.windowRect.height = window.height
        Setting.windowRect.x = mousePos.x
        Setting.windowRect.y = mousePos.y

        Setting.writeData()
    }

    StackView{
        id:centerView
        height:parent.height - bottomView.height
        width:parent.width - 20
        anchors.left:parent.left
        anchors.right:parent.right
        anchors.leftMargin:10
        anchors.rightMargin:10
        anchors.top:parent.top
        anchors.topMargin:10

        PageMain{
            id:mainPage
            visible: false
        }

        Item{
            id: editPage
            visible: false

            ViewLeftBar{
                height:parent.height
                width:200
                id:leftView
            }

            //中间内容导航
            ViewRightConter{
                id:mainView
                width: parent.width - leftView.width
                height: leftView.height
                anchors.left:leftView.right
            }
        }

        initialItem: editPage
    }

    //底部导航
    ViewBottomBar{
        id:bottomView
        height:90
        width:parent.width
        anchors.top:centerView.bottom
    }

    ViewPlayingTable {
        id: playingTable
        width: parent.width * 2 / 3
        height: parent.height - bottomView.height
        y:10
    }

    //对话框
    PopupMessage{
        id: popupMessage
    }

    ToolEditMusic{
        id:toolEditMusic
        width:600
        height:600
    }

    //计算长度
    TextMetrics{
        text: qsTr("00:00")
        font: Setting.mainFont
        onBoundingRectChanged: {
            Setting.timeWidth = boundingRect.width
        }
    }
    TextMetrics{
        text: qsTr("00-00-00 00:00:0000")
        font: Setting.mainFont
        onBoundingRectChanged: {
            Setting.editTimeWidth = boundingRect.width
        }
    }
    TextMetrics{
        text: qsTr("000000000")
        font: Setting.mainFont
        onBoundingRectChanged: {
            Setting.playNumberWidth = boundingRect.width
        }
    }

    //切换到主页
    function stackMain(){
        if(centerView.currentItem != mainPage){
            mainPage.show = true
            centerView.pop(null)
            centerView.pushItem(mainPage)
        }
    }

    //切换到编辑页
    function stackCenter(){
        if(centerView.currentItem != editPage){
            mainPage.show = false
            centerView.pop(null)
            centerView.pushItem(editPage)
        }
    }
}
