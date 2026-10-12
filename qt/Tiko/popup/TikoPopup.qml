import QtQuick
import QtQuick.Window
import QtQuick.Effects
import Tiko

Window {
    id: popupWindow
    color: "transparent"
    flags: Qt.Tool | Qt.FramelessWindowHint | Qt.NoDropShadowWindowHint
    modality: Qt.NonModal
    default property alias contentData: popupContent.data

    property string direction: "bottom"
    property real pointerSize: 12
    property real radius: 8
    property real borderWidth: 1
    property color backgroundColor: TikoSeit.theme.colorBgView
    property color borderColor: TikoSeit.theme.colorFgHint
    property color shadowColor: TikoSeit.theme.colorFgDefault
    property real shadowBlur: 0.5
    property real pointerOffset: 0
    property real contentMargin: 0
    property bool contentClip: false
    property Item anchorItem: null
    property real anchorX: 0
    property real anchorY: 0
    property real anchorOffsetX: 0
    property real anchorOffsetY: 0
    property Item dismissLayer: null
    property bool wasActive: false
    readonly property Item content: popupContent

    function updatePosition() {
        if (anchorItem === null)
            return

        const position = anchorItem.mapToGlobal(anchorX, anchorY)
        x = position.x + anchorOffsetX
        y = position.y + anchorOffsetY
    }

    function open() {
        updatePosition()
        TikoPopupManager.show(popupWindow)
        if (dismissLayer === null && transientParent !== null) {
            dismissLayer = dismissLayerComponent.createObject(transientParent.contentItem)
        }
        show()
        requestActivate()
    }

    function clearDismissLayer() {
        if (dismissLayer !== null) {
            dismissLayer.destroy()
            dismissLayer = null
        }
    }

    onClosing: {
        clearDismissLayer()
        TikoPopupManager.hide(popupWindow)
    }
    onVisibleChanged: {
        if (!visible) {
            clearDismissLayer()
            TikoPopupManager.hide(popupWindow)
        }
    }
    onActiveChanged: {
        if (active) {
            wasActive = true
        } else if (wasActive && visible) {
            close()
        }
    }
    Component.onDestruction: {
        clearDismissLayer()
        TikoPopupManager.hide(popupWindow)
    }

    Connections {
        target: popupWindow.anchorItem

        function onXChanged() { popupWindow.updatePosition() }
        function onYChanged() { popupWindow.updatePosition() }
        function onWidthChanged() { popupWindow.updatePosition() }
        function onHeightChanged() { popupWindow.updatePosition() }
    }

    Connections {
        target: popupWindow.anchorItem === null ? null : popupWindow.anchorItem.Window.window

        function onXChanged() { popupWindow.updatePosition() }
        function onYChanged() { popupWindow.updatePosition() }
    }

    Component {
        id: dismissLayerComponent

        MouseArea {
            anchors.fill: parent
            z: Number.MAX_VALUE
            acceptedButtons: Qt.AllButtons
            propagateComposedEvents: true
            onClicked: function(mouse) {
                popupWindow.close()
                mouse.accepted = false
            }
        }
    }

    Item {
        id: popupBackground
        anchors.fill: parent

        Canvas {
            id: surface
            anchors.fill: parent
            antialiasing: true

            onPaint: {
                const context = getContext("2d")
                const borderWidth = popupWindow.borderWidth
                const halfBorderWidth = borderWidth / 2
                const width = popupWindow.width - borderWidth
                const height = popupWindow.height - borderWidth
                const radius = popupWindow.radius
                const pointerSize = popupWindow.pointerSize
                const centerX = width / 2 + popupWindow.pointerOffset
                const centerY = height / 2 + popupWindow.pointerOffset

                context.clearRect(0, 0, popupWindow.width, popupWindow.height)
                context.beginPath()
                context.translate(halfBorderWidth, halfBorderWidth)

                if (popupWindow.direction === "top") {
                    context.moveTo(radius, pointerSize)
                    context.lineTo(centerX - pointerSize, pointerSize)
                    context.quadraticCurveTo(centerX - pointerSize * 0.4, pointerSize, centerX, 0)
                    context.quadraticCurveTo(centerX + pointerSize * 0.4, pointerSize, centerX + pointerSize, pointerSize)
                    context.lineTo(width - radius, pointerSize)
                    context.quadraticCurveTo(width, pointerSize, width, radius + pointerSize)
                    context.lineTo(width, height - radius)
                    context.quadraticCurveTo(width, height, width - radius, height)
                    context.lineTo(radius, height)
                    context.quadraticCurveTo(0, height, 0, height - radius)
                    context.lineTo(0, radius + pointerSize)
                    context.quadraticCurveTo(0, pointerSize, radius, pointerSize)
                } else if (popupWindow.direction === "bottom") {
                    context.moveTo(radius, 0)
                    context.lineTo(width - radius, 0)
                    context.quadraticCurveTo(width, 0, width, radius)
                    context.lineTo(width, height - radius - pointerSize)
                    context.quadraticCurveTo(width, height - pointerSize, width - radius, height - pointerSize)
                    context.lineTo(centerX + pointerSize, height - pointerSize)
                    context.quadraticCurveTo(centerX + pointerSize * 0.4, height - pointerSize, centerX, height)
                    context.quadraticCurveTo(centerX - pointerSize * 0.4, height - pointerSize, centerX - pointerSize, height - pointerSize)
                    context.lineTo(radius, height - pointerSize)
                    context.quadraticCurveTo(0, height - pointerSize, 0, height - radius - pointerSize)
                    context.lineTo(0, radius)
                    context.quadraticCurveTo(0, 0, radius, 0)
                } else if (popupWindow.direction === "left") {
                    context.moveTo(pointerSize + radius, 0)
                    context.lineTo(width - radius, 0)
                    context.quadraticCurveTo(width, 0, width, radius)
                    context.lineTo(width, height - radius)
                    context.quadraticCurveTo(width, height, width - radius, height)
                    context.lineTo(pointerSize + radius, height)
                    context.quadraticCurveTo(pointerSize, height, pointerSize, height - radius)
                    context.lineTo(pointerSize, centerY + pointerSize)
                    context.quadraticCurveTo(pointerSize, centerY + pointerSize * 0.4, 0, centerY)
                    context.quadraticCurveTo(pointerSize, centerY - pointerSize * 0.4, pointerSize, centerY - pointerSize)
                    context.lineTo(pointerSize, radius)
                    context.quadraticCurveTo(pointerSize, 0, pointerSize + radius, 0)
                } else {
                    context.moveTo(radius, 0)
                    context.lineTo(width - pointerSize - radius, 0)
                    context.quadraticCurveTo(width - pointerSize, 0, width - pointerSize, radius)
                    context.lineTo(width - pointerSize, centerY - pointerSize)
                    context.quadraticCurveTo(width - pointerSize, centerY - pointerSize * 0.4, width, centerY)
                    context.quadraticCurveTo(width - pointerSize, centerY + pointerSize * 0.4, width - pointerSize, centerY + pointerSize)
                    context.lineTo(width - pointerSize, height - radius)
                    context.quadraticCurveTo(width - pointerSize, height, width - pointerSize - radius, height)
                    context.lineTo(radius, height)
                    context.quadraticCurveTo(0, height, 0, height - radius)
                    context.lineTo(0, radius)
                    context.quadraticCurveTo(0, 0, radius, 0)
                }

                context.closePath()
                context.fillStyle = popupWindow.backgroundColor
                context.fill()
                context.lineWidth = borderWidth
                context.strokeStyle = popupWindow.borderColor
                context.stroke()
            }
        }

        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: popupWindow.shadowBlur
            shadowColor: popupWindow.shadowColor
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }
    }

    Item {
        id: popupContent
        anchors.fill: parent
        clip: popupWindow.contentClip
        anchors.leftMargin: (popupWindow.direction === "left" ? popupWindow.pointerSize : 0)
                            + popupWindow.radius + popupWindow.contentMargin
        anchors.rightMargin: (popupWindow.direction === "right" ? popupWindow.pointerSize : 0)
                             + popupWindow.radius + popupWindow.contentMargin
        anchors.topMargin: (popupWindow.direction === "top" ? popupWindow.pointerSize : 0)
                           + popupWindow.radius + popupWindow.contentMargin
        anchors.bottomMargin: (popupWindow.direction === "bottom" ? popupWindow.pointerSize : 0)
                              + popupWindow.radius + popupWindow.contentMargin
    }

    Shortcut {
        sequence: "Escape"
        context: Qt.WindowShortcut
        onActivated: popupWindow.close()
    }

    onWidthChanged: surface.requestPaint()
    onHeightChanged: surface.requestPaint()
    onDirectionChanged: surface.requestPaint()
    onPointerOffsetChanged: surface.requestPaint()
    onPointerSizeChanged: surface.requestPaint()
    onRadiusChanged: surface.requestPaint()
    onBorderWidthChanged: surface.requestPaint()
    onBorderColorChanged: surface.requestPaint()
    onBackgroundColorChanged: surface.requestPaint()
}
