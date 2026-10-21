import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import MediaerAPI
import Tiko

Loader {
    id: coreLrcLine

    property color playingColor: TikoSeit.theme.colorFgDefault
    property color normalColor: TikoSeit.theme.colorFgHint
    property int lrcId: -1
    property int startTime: 0
    property int duration: 0
    property bool hovered: mouseAreaItem.containsMouse
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []

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
            hovered: coreLrcLine.hovered

            playingColor: coreLrcLine.playingColor
            normalColor: coreLrcLine.normalColor
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

            playingColor: coreLrcLine.playingColor
            normalColor: coreLrcLine.normalColor
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

