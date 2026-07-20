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
    property color unifiedColor: TikoSeit.theme.colorFgDefault

    layer.enabled: true
    layer.effect: MultiEffect {
        colorization: enableUnifiedColor ? 1 : 0
        colorizationColor: unifiedColor
    }

    // 监听变化
    onWidthChanged: syncTimer.restart()
    onHeightChanged: syncTimer.restart()

    Timer {
        id: syncTimer
        interval: 200 // 延迟 200 毫秒同步，避开频繁的动画过程
        repeat: false
        onTriggered: {
            // 只有停止变动后，才真正设置采样尺寸
            imageColorAutoView.sourceSize.width = imageColorAutoView.width
            imageColorAutoView.sourceSize.height = imageColorAutoView.height
        }
    }

    opacity: status === Image.Ready ? 1 : 0
    Behavior on opacity {
        NumberAnimation { duration: 50 }
    }
}
