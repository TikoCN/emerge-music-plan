import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView

//纯色底片
AutoCoverImage{
    id: backCover
    anchors.fill: parent
    visible: false
    loadMsTime: 10

    layer.enabled: true
    layer.effect: MultiEffect {
        id: effectCover
        autoPaddingEnabled: true
        source: backCover
        blurEnabled: true
        blurMax: 10
        blur: 1.0
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentAntiEmphasize
    }
}
