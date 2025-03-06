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

    Image {
        id: backCover
        anchors.fill: pageMain
        sourceSize.width: backCover.width
        sourceSize.height: backCover.height
        visible: false
    }

    MultiEffect {
        autoPaddingEnabled: true
        source: backCover
        anchors.fill: backCover
        blurEnabled: true
        blurMax: 64
        blur: 1.0
    }
    Rectangle{
        anchors.fill: backCover
        color: Setting.backdropColor
        opacity: 0.3
    }

    //关闭
    TikoUiButton{
        id: close
        anchors.right: parent.right
        text: qsTr("关闭")
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
    }

    //最大化
    TikoUiButton{
        id: max
        anchors.right: close.left
        anchors.rightMargin: 20
        text: qsTr("最大化")
        icon.source: "qrc:/image/max.png"
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
    TikoUiButton{
        id: min
        anchors.right: max.left
        anchors.rightMargin: 20
        text: qsTr("最小化")
        icon.source: "qrc:/image/min.png"
        onClicked: window.showMinimized()
    }

    //返回主页
    TikoUiButton{
        id: back
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: qsTr("返回")
        icon.source: "qrc:/image/back.png"
        onClicked: window.stackCenter()
    }

    Loader{
        id: loaderStyle
        width: pageMain.width
        height: pageMain.height - back.height
        y: back.height
    }

    Connections{
        target:MediaPlayer.player
        function onSourceChanged(){
            backCover.source = "image://cover/back:" + MediaPlayer.playingCore.coreId.toString()
        }
    }

    Component.onCompleted: {
        backCover.source = "image://cover/back:" + MediaPlayer.playingCore.coreId.toString()
    }
}
