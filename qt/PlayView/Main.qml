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

    // 背景与阴影实现
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

        ViewLeftBar{
            id: barView
            height: parent.height
            width: 250
        }

        //中间内容导航
        ViewMain{
            id: mainView
            height: parent.height
            width: parent.width - barView.width - 10
            x: barView.width + 10
        }

        property int duration: 500
        property int inType: Easing.InOutQuad
        property int outType: Easing.InOutQuad

        //正在播放列表
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
    }

    //切换到编辑页
    function stackCenter(){
    }

    //清理数据
    function clearData(){
        playingPlayList.clearData()
        mainView.stackDetail()
    }

    Connections{
        target: DataActive
        function onFinish(){
            CoreData.playlist = JSON.parse(PlayListLibrary.getAllList())
        }
    }

    // 程序开始
    Component.onCompleted: {
        CoreData.playlist = JSON.parse(PlayListLibrary.getAllList())
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
}
