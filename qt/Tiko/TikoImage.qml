import QtQuick.Effects
import QtQuick

Image{
    id: imageColorAutoView
    sourceSize.width: width
    sourceSize.height: height
    width: 32
    height: 32
    asynchronous: true
    cache: true
    mipmap: true
    fillMode: Image.PreserveAspectFit
    smooth: true

    property bool isUseAutoColor: true
    property color autoColor: Qt.red

    MultiEffect {
        id: iconShow
        anchors.fill: imageColorAutoView
        source: imageColorAutoView
        colorization: isUseAutoColor ? 1 : 0
        colorizationColor: autoColor
    }
}

