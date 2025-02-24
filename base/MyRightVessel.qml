import QtQuick
import MyAPI

Item {
    id: root
    property MyAutoText show: show
    property Component vessel
    height: show.height > bgLoader.height ? show.height + 20 : bgLoader.height + 20

    MyAutoText{
        id: show
        x: 10
        y: 10
        exSize: 2
        opacity: 0.3
    }

    // 动态加载背景组件
    Loader {
        id: bgLoader
        x: show.x + show.width + 10
        y: show.y
        width: parent.width - x - 10
        sourceComponent: root.vessel
    }

    Rectangle{
        width: parent.width
        height: 3
        color: Setting.themeColor
        opacity: 0.3
        radius: 1.5
    }
}
