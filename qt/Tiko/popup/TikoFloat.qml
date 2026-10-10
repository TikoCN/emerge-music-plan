import QtQuick
import QtQuick.Effects
import Tiko

Item {
    id: floatItem
    visible: false
    z: 1000

    property string direction: "bottom"
    property real pointerSize: 12
    property real radius: 8
    property real borderWidth: 1
    property color backgroundColor: TikoSeit.theme.colorBgView
    property color borderColor: TikoSeit.theme.colorFgHint
    property color shadowColor: TikoSeit.theme.colorFgDefault
    property real shadowBlur: 0.5
    property real pointerOffset: 0
    property Item dismissLayer: null
    default property alias contentData: contentItem.data
    readonly property Item content: contentItem

    function open() {
        TikoFloatManager.show(floatItem)
        if (dismissLayer === null && parent !== null) {
            dismissLayer = dismissLayerComponent.createObject(parent, {"ownerFloat": floatItem})
        }
        visible = true
        z = Number.MAX_VALUE
    }

    function close() {
        visible = false
        if (dismissLayer !== null) {
            dismissLayer.destroy()
            dismissLayer = null
        }
        TikoFloatManager.hide(floatItem)
    }

    onWidthChanged: {
        surface.requestPaint()
    }
    onHeightChanged: {
        surface.requestPaint()
    }
    onDirectionChanged: {
        surface.requestPaint()
    }
    Component.onDestruction: TikoFloatManager.hide(floatItem)

    Component {
        id: dismissLayerComponent

        MouseArea {
            anchors.fill: parent
            z: Number.MAX_VALUE / 2
            property Item ownerFloat: null
            onClicked: ownerFloat.close()
        }
    }

    Canvas {
        id: surface
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            const context = getContext("2d")
            const width = floatItem.width
            const height = floatItem.height
            const radius = floatItem.radius
            const pointerSize = floatItem.pointerSize
            const centerX = width / 2 + floatItem.pointerOffset
            const centerY = height / 2 + floatItem.pointerOffset

            context.clearRect(0, 0, width, height)
            context.beginPath()

            if (floatItem.direction === "top") {
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
            } else if (floatItem.direction === "bottom") {
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
            } else if (floatItem.direction === "left") {
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
            context.fillStyle = floatItem.backgroundColor
            context.fill()
            context.lineWidth = floatItem.borderWidth
            context.strokeStyle = floatItem.borderColor
            context.stroke()
        }
    }

    Item {
        id: contentItem
        x: floatItem.direction === "left" ? floatItem.pointerSize + floatItem.radius : floatItem.radius
        y: floatItem.direction === "top" ? floatItem.pointerSize + floatItem.radius : floatItem.radius
        width: floatItem.width - floatItem.radius * 2 - ((floatItem.direction === "left" || floatItem.direction === "right") ? floatItem.pointerSize : 0)
        height: floatItem.height - floatItem.radius * 2 - ((floatItem.direction === "top" || floatItem.direction === "bottom") ? floatItem.pointerSize : 0)
    }

    layer.enabled: true
    layer.effect: MultiEffect {
        shadowEnabled: true
        shadowBlur: floatItem.shadowBlur
        shadowColor: floatItem.shadowColor
        shadowHorizontalOffset: 0
        shadowVerticalOffset: 0
    }

    onPointerOffsetChanged: surface.requestPaint()
    onPointerSizeChanged: surface.requestPaint()
    onRadiusChanged: surface.requestPaint()
    onBorderWidthChanged: surface.requestPaint()
    onBorderColorChanged: surface.requestPaint()
    onBackgroundColorChanged: surface.requestPaint()
}
