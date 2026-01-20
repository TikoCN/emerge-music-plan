import QtQuick.Effects
import QtQuick

Image{
    id: imageColorAutoView
    sourceSize.width: width
    sourceSize.height: height
    asynchronous: true
    cache: true
    mipmap: true
    fillMode: Image.PreserveAspectFit
    smooth: true
    property bool isAutoColor: true

    MultiEffect {
        id: iconShow
        anchors.fill: imageColorAutoView
        source: imageColorAutoView
        colorization: isAutoColor ? 1 : 0
        colorizationColor: TikoSeit.transparentColor
        enabled: imageColorAutoView.visible
    }
}

