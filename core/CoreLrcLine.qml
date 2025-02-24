import QtQuick
import QtQuick.Controls.Basic
import MyAPI
import "../base"

Item {
    id: coreLrcLine
    height: textMetrics.boundingRect.height * lrcData.line
    property font lrcFont
    property var lrcData
    property bool isUse: true

    Connections{
        target: lrcData
        function onPosChanged(){
            if(textMetrics.boundingRect.width < width){
                moveBar.position = 0
                return;//不超出长度，退出
            }

            var shwoLine = lrcData.pos * textMetrics.boundingRect.width
            if(shwoLine > width * 2 / 3){
                moveBar.position = (lrcData.pos * textMetrics.boundingRect.width - width* 2 / 3) / textMetrics.boundingRect.width
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
        ScrollBar.horizontal:MyBar{
            id: moveBar
            visible: false
            width: 0
        }

        Text{
            id: lrcPlayedText
            text: lrcPlayingText.text
            anchors.left: parent.left
            color: Setting.playedLrcColor
            font.family: lrcFont.family
            font.bold: true
            font.pixelSize: lrcData.isPlay ? lrcFont.pixelSize + 5 : lrcFont.pixelSize
            width: textMetrics.boundingRect.width
        }

        Text{
            id: lrcPlayingText
            text: lrcData.text
            anchors.left: parent.left
            color: Setting.playingLrcColor
            font: lrcPlayedText.font
            clip: true
            width: lrcData.pos * textMetrics.boundingRect.width
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
                          MediaPlayer.turnToLrc(lrcData.id)
                      }
                      mouse.accepted = false
                  }
    }
}

