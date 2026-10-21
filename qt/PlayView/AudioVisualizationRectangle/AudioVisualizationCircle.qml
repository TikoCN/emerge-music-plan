import QtQuick
import PlayView
import MediaerAPI
import Tiko

Item {
    id: canvas
    property color showColor: TikoSeit.theme.colorFgDefault
    property int startAngle: 0
    property real r: Math.min(canvas.width, canvas.height) * 0.3

    SpectrumItem {
        id: visualizer
        anchors.fill: parent
        mode: SpectrumItem.Circular
        color: canvas.showColor
        radius: canvas.r
        startAngle: canvas.startAngle

        Component.onCompleted: samples = MediaPlayer.allSamples
    }

    Connections {
        target: MediaPlayer
        function onBufferSampleChanged() {
            visualizer.samples = MediaPlayer.allSamples
        }
    }
}
