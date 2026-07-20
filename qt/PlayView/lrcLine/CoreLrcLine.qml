import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import MediaerAPI
import Tiko

Loader {
    id: coreLrcLine

    property color playingColor: "#ffffffff"
    property color normalColor: "#80ffffff"
    property int lrcId: -1
    property var wordList: []
    property int startTime: lrc.startTime
    property int duration: lrc.endTime
    property var startList: lrc.startList
    property var endList: lrc.endList
    property var textList: lrc.textList
    property var helpTextList: lrc.helpTextList
    property lrcData lrc

    onLrcIdChanged: lrc = MediaPlayer.getLrcData(lrcId)
    sourceComponent:  textList.length === 0 ? drawLoadLineCom : drawLrcLineCom

    Component {
        id: drawLrcLineCom
        LrcLineNormal {
            width: coreLrcLine.width
            id: drawLrcLine
            lrcId: coreLrcLine.lrcId
            startList: coreLrcLine.startList
            endList: coreLrcLine.endList
            textList: coreLrcLine.textList
            helpTextList: coreLrcLine.helpTextList

            playingColor: TikoSeit.theme.colorTextHighlight
            normalColor: TikoSeit.theme.colorTextHint
        }
    }

    Component {
        id: drawLoadLineCom
        LrcLineLoad {
            width: coreLrcLine.width
            id: drawLoadLine
            lrcId: coreLrcLine.lrcId
            startTime: coreLrcLine.startTime
            duration: coreLrcLine.duration

            playingColor: TikoSeit.theme.colorTextHighlight
            normalColor: TikoSeit.theme.colorTextHint
        }
    }

    MouseArea{
        id: mouseAreaItem
        anchors.fill: parent
        propagateComposedEvents: true
        acceptedButtons: Qt.RightButton | Qt.LeftButton
        hoverEnabled: true

        onClicked: (mouse)=>{
                       //跳转到当前时间
                       if(mouse.button === Qt.LeftButton){
                           MediaPlayer.turnToLrc(lrcId)
                       }
                       mouse.accepted = false
                   }
    }
}

