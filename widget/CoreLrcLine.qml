import QtQuick
import QtQuick.Controls.Basic
import TikoAPI
import Tiko

Item {
    id: coreLrcLine
    height: textMetrics.boundingRect.height
    property font lrcFont
    property double pos: 0
    property string text: qsTr("歌词加载中")
    property bool isUse: true
    property int lrcId: 0//歌词序号

    Connections{
        target: coreLrcLine
        function onPosChanged(){
            if(textMetrics.boundingRect.width < width){
                moveBar.position = 0
                return;//不超出长度，退出
            }

            var shwoLine = coreLrcLine.pos * textMetrics.boundingRect.width
            if(shwoLine > width * 2 / 3){
                moveBar.position = (coreLrcLine.pos * textMetrics.boundingRect.width - width* 2 / 3) / textMetrics.boundingRect.width
            }
            else if(shwoLine < width){
                moveBar.position = 0
            }
        }
    }

    ScrollView{
        id: moveArea
        anchors.fill: parent
        ScrollBar.vertical.visible: false
        ScrollBar.horizontal:TikoBar{
            id: moveBar
            visible: false
            width: 0
        }

        Text{
            id: lrcPlayedText
            text: lrcPlayingText.text
            anchors.left: parent.left
            color: Setting.playedLrcColor
            font.family: coreLrcLine.lrcFont.family
            font.bold: true
            font.pixelSize: coreLrcLine.lrcFont.pixelSize
            width: textMetrics.boundingRect.width
        }

        Text{
            id: lrcPlayingText
            text: coreLrcLine.text
            anchors.left: parent.left
            color: Setting.playingLrcColor
            font: lrcPlayedText.font
            clip: true
            width: coreLrcLine.pos * textMetrics.boundingRect.width
        }
    }

    TextMetrics{
        id: textMetrics
        font: lrcPlayingText.font
        text: lrcPlayingText.text.split("\n")[0]
    }

    MouseArea{
        anchors.fill: parent
        propagateComposedEvents: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton

        onClicked:(mouse)=>{
                      //跳转到当前时间
                      if(mouse.button === Qt.LeftButton && isUse){
                          MediaPlayer.turnToLrc(coreLrcLine.lrcId)
                      }
                      mouse.accepted = false
                  }
    }
}

