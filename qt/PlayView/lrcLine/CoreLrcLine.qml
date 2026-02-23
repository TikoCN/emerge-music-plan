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
    property int startTime: -1
    property int duration: -1
    property var startList: []
    property var endList: []
    property var textList: []
    property var helpTextList: []

    onLrcIdChanged: {
        const json = MediaPlayer.getLrcJsonObject(lrcId);
        startList = BaseTool.typeConversion.stringToLongList(json.startList)
        endList = BaseTool.typeConversion.stringToLongList(json.endList)
        textList = BaseTool.typeConversion.stringToStringList(json.textList)
        helpTextList = BaseTool.typeConversion.stringToStringList(json.helpTextList)
        startTime = Number(json.startTime)
        duration = Number(json.endTime - json.startTime)
        if (textList.length > 0) {
            sourceComponent = drawLrcLineCom
        } else {
            sourceComponent = drawLoadLineCom
        }
    }

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

