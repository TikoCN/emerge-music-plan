import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import MediaerAPI
import Tiko

Item {
    id: coreLrcLine
    height: loader.item ? loader.item.height : 0

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

    Component.onCompleted: {
        const json = MediaPlayer.getLrcJsonObject(lrcId);
        startList = BaseTool.typeConversion.stringToLongList(json.startList)
        endList = BaseTool.typeConversion.stringToLongList(json.endList)
        textList = BaseTool.typeConversion.stringToStringList(json.textList)
        helpTextList = BaseTool.typeConversion.stringToStringList(json.helpTextList)
        startTime = Number(json.startTime)
        duration = Number(json.endTime - json.startTime)
    }

    Loader {
        id: loader
        anchors.fill: parent
        sourceComponent: textList.length > 0 ? drawLrcLineCom : drawLoadLineCom
    }

    Component {
        id: drawLrcLineCom
        DrawLrcLine {
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
        DrawLoadLine {
            width: coreLrcLine.width
            id: drawLoadLine
            lrcId: coreLrcLine.lrcId
            startTime: coreLrcLine.startTime
            duration: coreLrcLine.duration
        }
    }
}

