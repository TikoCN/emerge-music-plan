import QtQuick.Controls.Basic
import QtQuick

ScrollBar {
    id: root
    height: parent.height
    size: 1
    width: 10
    anchors.right: parent.right
    snapMode: ScrollBar.SnapAlways
    policy: ScrollBar.AsNeeded

    background: Rectangle {  // 背景
        id: background
        anchors.fill: parent
        color: root.active ? TikoSeit.themeColor : TikoSeit.backdropColor
        opacity: 0.3
        radius: 4
    }

    contentItem: Rectangle {  // 滑块
        id: handle
        color: TikoSeit.themeColor // 颜色
        radius: 3
    }
}
