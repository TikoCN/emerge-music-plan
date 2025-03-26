import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Effects
import TikoAPI
import Tiko

Item {
    id: coreLrcLine
    height: loader.item ? loader.item.height : 0

    property color playingColor: "#ffffffff"
    property color normalColor: "#80ffffff"
    property var core
    property bool isUse: false

   Loader {
       id: loader
       anchors.fill: parent
       sourceComponent: core.textList.length > 0 ? drawLrcLineCom : drawLoadLineCom
   }

   Component {
       id: drawLrcLineCom
       DrawLrcLine {
           width: coreLrcLine.width
           id: drawLrcLine
           core: coreLrcLine.core
       }
   }

   Component {
       id: drawLoadLineCom
       DrawLoadLine {
           width: coreLrcLine.width
           id: drawLoadLine
           core: coreLrcLine.core
       }
   }
}

