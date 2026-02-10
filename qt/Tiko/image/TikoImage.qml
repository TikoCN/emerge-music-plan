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

    property bool enableUnifiedColor: true
    property color unifiedColor: Qt.red

    MultiEffect {
        id: iconShow
        anchors.fill: imageColorAutoView
        source: imageColorAutoView
        colorization: enableUnifiedColor ? 1 : 0
        colorizationColor: unifiedColor
    }
}

