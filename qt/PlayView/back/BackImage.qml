import QtQuick.Controls.Basic
import QtQuick
import QtQuick.Effects
import MediaerAPI
import Tiko
import PlayView

//纯色底片
Rectangle{
    color: TikoSeit.theme.baseTheme.backgroundNormal
    property string iconUrl: ""
    clip: true

    //模糊背景
    AutoCoverImage {
        id: backCover
        anchors.fill: parent
        loadMsTime: 1
        visible: false
        loadFlag: true
        baseUrl: iconUrl
    }

    MultiEffect {
        id: effectCover
        autoPaddingEnabled: true
        source: backCover
        width: parent.width * 1.5
        height: parent.height * 1.5
        blurEnabled: true
        blurMax: 50
        blur: 1.0
    }

    Rectangle{
        anchors.fill: parent
        color: TikoSeit.theme.baseTheme.transparentAntiEmphasize
    }
}
