import QtQuick
import QtQuick.Window
import QtQuick.Effects

Window {
    id: win

    property bool sizeChange: false

    color: Qt.rgba(0,0,0,0)
    flags: Qt.Window | Qt.FramelessWindowHint // 尝试隐藏边框和标题栏

    Rectangle {
        id: windowSurface
        z: -2
        anchors.fill: parent
        anchors.margins: win.visibility === Window.Maximized ? 0 : 10
        color: TikoSeit.theme.colorBgView
        radius: win.visibility === Window.Maximized ? 0 : 10
        layer.enabled: win.visibility !== Window.Maximized
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowBlur: 0.5
            shadowColor: TikoSeit.theme.colorMaxTop
            shadowHorizontalOffset: 0
            shadowVerticalOffset: 0
        }
    }

    MouseArea {
        z: -1
        width: parent.width - 32
        height: 8
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        cursorShape: Qt.SizeVerCursor
        onPressed: {
            win.startSystemResize(Qt.TopEdge)
            win.sizeChange = true
        }
        onReleased: win.sizeChange = false
    }

    MouseArea {
        z: -1
        width: parent.width - 32
        height: 8
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        cursorShape: Qt.SizeVerCursor
        onPressed: {
            win.startSystemResize(Qt.BottomEdge)
            win.sizeChange = true
        }
        onReleased: win.sizeChange = false
    }

    MouseArea {
        z: -1
        width: 8
        height: parent.height - 32
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        cursorShape: Qt.SizeHorCursor
        onPressed: {
            win.startSystemResize(Qt.LeftEdge)
            win.sizeChange = true
        }
        onReleased: win.sizeChange = false
    }

    MouseArea {
        z: -1
        width: 8
        height: parent.height - 32
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        cursorShape: Qt.SizeHorCursor
        onPressed: {
            win.startSystemResize(Qt.RightEdge)
            win.sizeChange = true
        }
        onReleased: win.sizeChange = false
    }

    MouseArea {
        z: -1
        width: 16
        height: 16
        anchors.top: parent.top
        anchors.left: parent.left
        cursorShape: Qt.SizeFDiagCursor
        onPressed: win.startSystemResize(Qt.TopEdge | Qt.LeftEdge)
    }

    MouseArea {
        z: -1
        width: 16
        height: 16
        anchors.top: parent.top
        anchors.right: parent.right
        cursorShape: Qt.SizeBDiagCursor
        onPressed: win.startSystemResize(Qt.TopEdge | Qt.RightEdge)
    }

    MouseArea {
        z: -1
        width: 16
        height: 16
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        cursorShape: Qt.SizeBDiagCursor
        onPressed: win.startSystemResize(Qt.BottomEdge | Qt.LeftEdge)
    }

    MouseArea {
        z: -1
        width: 16
        height: 16
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        cursorShape: Qt.SizeFDiagCursor
        onPressed: win.startSystemResize(Qt.BottomEdge | Qt.RightEdge)
    }
}
