import QtQuick
import QtQuick.Effects
import TikoAPI
import Tiko
import Widget

Item {
    id: pageMain
    property bool show: false
    property int type: 1

    onShowChanged: {
        if(!pageMain.show){
            loaderStyle.source = ""
            return
        }

        switch(pageMain.type){
        case 0:
            loaderStyle.source = "./mainStyle/MainStyle0.qml"
            break
        case 1:
            loaderStyle.source = "./mainStyle/MainStyle1.qml"
            break
        case 2:
            loaderStyle.source = "./mainStyle/MainStyle2.qml"
            break
        case 3:
            loaderStyle.source = "./mainStyle/MainStyle3.qml"
            break
        case 4:
            loaderStyle.source = "./mainStyle/MainStyle4.qml"
            break
        case 5:
            loaderStyle.source = "./mainStyle/MainStyle5.qml"
            break
        }
    }

    //纯色底片
    Rectangle{
        anchors.fill: pageMain
        color: Setting.transparentColor
    }
    //模糊背景
    Image {
        id: backCover
        width: 100
        height: 100
        sourceSize.width: backCover.width
        sourceSize.height: backCover.height
        visible: false
    }
    MultiEffect {
        id: effectCover
        autoPaddingEnabled: true
        source: backCover
        width: pageMain.width * 5
        height: pageMain.height * 5
        blurEnabled: true
        blurMax: 50
        blur: 1.0

        property int stepX: getRand()
        property int stepY: getRand()
        property bool addX: true
        property bool addY: true

        function getRand(){
            return Math.floor(Math.random() * 5 + 2)
        }

        function move(){
            var minX = pageMain.width - effectCover.width
            var minY = pageMain.height - effectCover.height
            var maxX = 0
            var maxY = 0

            var moveX = addX ? effectCover.stepX : -effectCover.stepX
            var moveY = addY ? effectCover.stepY : -effectCover.stepY

            // 计算x方向
            if (effectCover.x === minX){
                effectCover.addX = true
                effectCover.stepX = getRand()
                moveX = effectCover.stepX
            }
            else if (effectCover.x === maxX){
                effectCover.addX = false
                effectCover.stepX = getRand()
                moveX = -effectCover.stepX
            }

            // 限制位置
            if (moveX > 0 && effectCover.x + moveX > maxX){
                moveX = maxX - effectCover.x
            }
            else if (moveX < 0 && effectCover.x + moveX < minX){
                moveX = minX - effectCover.x
            }

            // 计算y方向
            if (effectCover.y === minY){
                effectCover.addY = true
                effectCover.stepY = getRand()
                moveY = effectCover.stepY
            }
            else if (effectCover.y === maxY){
                effectCover.addY = false
                effectCover.stepY = getRand()
                moveY = -effectCover.stepY
            }

            // 限制位置
            if (moveY > 0 && effectCover.y + moveY > maxY){
                moveY = maxY - effectCover.y
            }
            else if (moveY < 0 && effectCover.y + moveY < minY){
                moveY = minY - effectCover.y
            }

            effectCover.x += moveX
            effectCover.y += moveY
        }
    }
    Timer {
        id: backMove
        interval: 33
        repeat: true
        onTriggered: effectCover.move()
    }

    //透明成显示
    Rectangle{
        anchors.fill: pageMain
        color: Setting.transparentColor
        opacity: 0.3
    }

    Loader{
        id: loaderStyle
        width: pageMain.width
        height: pageMain.height - back.height
        y: back.height
    }

    //关闭
    TikoButtonIcon{
        id: close
        anchors.right: parent.right
        text: qsTr("关闭")
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
        icon.width: 15
        icon.height: 15
    }

    //最大化
    TikoButtonIcon{
        id: max
        anchors.right: close.left
        anchors.rightMargin: 6
        text: qsTr("最大化")
        icon.source: "qrc:/image/max.png"
        icon.width: 15
        icon.height: 15
        onClicked: {
            if(window.visibility === 5){
                window.showNormal()
            }
            else{
                window.showFullScreen()
            }
        }
    }

    //最小化
    TikoButtonIcon{
        id: min
        anchors.right: max.left
        anchors.rightMargin: 6
        text: qsTr("最小化")
        icon.source: "qrc:/image/min.png"
        icon.width: 15
        icon.height: 15
        onClicked: window.showMinimized()
    }

    //返回主页
    TikoButtonIcon{
        id: back
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: qsTr("返回")
        icon.source: "qrc:/image/back.png"
        icon.width: 15
        icon.height: 15
        onClicked: window.stackCenter()
    }

    NumberAnimation on y {
        id: insertAnimation
        from: height / 2
        to: 0
        duration: 300
    }

    Connections{
        target:MediaPlayer.player
        function onSourceChanged(){
            backCover.source = "image://cover/back:" + MediaPlayer.playingMusic.coreId.toString()
        }
    }

    function actionStart(){
        insertAnimation.start()
        backMove.start()
        pageMain.show = true
    }

    function actionEnd() {
        backMove.stop()
        pageMain.show = false
    }

}
