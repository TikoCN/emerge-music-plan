import QtQuick
import QtQuick.Effects
import Tiko
Image {
    id: imageColorAutoView
    width: 20
    height: 20
    sourceSize.width: width
    sourceSize.height: height
    asynchronous: true
    cache: true
    mipmap: true
    fillMode: Image.PreserveAspectFit
    smooth: true

    property bool enableUnifiedColor: true
    property color unifiedColor: TikoSeit.theme.baseTheme.foregroundNormal

    layer.enabled: true
    layer.effect: MultiEffect {
        colorization: enableUnifiedColor ? 1 : 0
        colorizationColor: unifiedColor
    }
}
