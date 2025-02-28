import QtQuick
import TikoAPI
import "../base"
import "../core"

Item {
    id: pageMain
    property bool show:  false

    //关闭
    TikoButton{
        id: close
        anchors.right: parent.right
        text: qsTr("关闭")
        icon.source: "qrc:/image/close.png"
        onClicked: window.close()
    }

    //最大化
    TikoButton{
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
    TikoButton{
        id: min
        anchors.right: max.left
        anchors.rightMargin: 20
        text: qsTr("最小化")
        icon.source: "qrc:/image/min.png"
        onClicked: window.showMinimized()
    }

    //返回主页
    TikoButton{
        id: back
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: qsTr("最小化")
        icon.source: "qrc:/image/back.png"
        onClicked: window.stackCenter()
    }

    TikoAutoText{
        id: title
        width: cover.width + parent.width / 20
        height: 30
        anchors.bottom: artist.top
        anchors.left: cover.left
        font.bold: true
        exSize: 6
        text: qsTr("标题")
    }

    TikoAutoText{
        id: artist
        width: cover.width + parent.width / 20
        height: 30
        anchors.bottom: cover.top
        anchors.left: cover.left
        exSize: 3
        text: qsTr("作者")
    }

    Image {
        id: cover
        cache: false
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset:  -width/2 - parent.width / 20
        anchors.verticalCenter: parent.verticalCenter
        width: 300
        height: 300
        sourceSize.width: width
        sourceSize.height: height
        asynchronous: true
        source: "qrc:/image/cover.png"
    }

    //滚动歌词
    PlayerLrcTable{
        id: lrcShow
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: width/2 + parent.width / 20
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 3
        height: title.height + artist.height + cover.height
        show: pageMain.show
    }

    PlayerSpectrum{
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        height: parent.height / 2 - lrcShow.height / 2
        width: parent.width * 2 / 3
    }

    //关联
    Connections{
        target:MediaPlayer.player
        function onSourceChanged(){
            artist.text = MediaPlayer.playingCore.artist
            title.text = MediaPlayer.playingCore.title
            cover.source = "image://cover/onLine:" + MediaPlayer.playingCore.coreId.toString()
        }
    }
}
