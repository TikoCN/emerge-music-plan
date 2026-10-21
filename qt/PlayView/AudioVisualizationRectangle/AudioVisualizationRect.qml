import QtQuick
import PlayView
import MediaerAPI
import Tiko

Item {
    id: canvas
    property color showColor: TikoSeit.theme.colorFgDefault

    SpectrumItem {
        id: visualizer
        anchors.fill: parent
        mode: SpectrumItem.Waveform
        color: canvas.showColor

        Component.onCompleted: samples = MediaPlayer.allSamples
    }

    Connections {
        target: MediaPlayer
        function onBufferSampleChanged() {
            visualizer.samples = MediaPlayer.allSamples
        }
    }
}
