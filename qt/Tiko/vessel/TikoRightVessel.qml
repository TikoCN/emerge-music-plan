import QtQuick
import Tiko

Item {
    id: rightVessel
    height: rightVessel.itemShow && titleButtonItem.height < (bgLoader.height || 0) ?
                (bgLoader.height || 0) + 50 : titleButtonItem.implicitHeight + 50
    property TikoButtonNormal titleButton: titleButtonItem
    property Component vessel
    property bool itemShow: true

    TikoButtonNormal {
        id: titleButtonItem
        x: 10
        y: 25
        onLeftClicked: rightVessel.itemShow = !rightVessel.itemShow
    }

    // 动态加载背景组件
    Loader {
        id: bgLoader
        anchors.left: titleButtonItem.right
        anchors.top: titleButtonItem.top
        anchors.right: parent.right
        anchors.rightMargin: TikoSeit.normalMargins
        anchors.leftMargin: TikoSeit.normalMargins

        sourceComponent: rightVessel.vessel
        visible: rightVessel.itemShow

        onItemChanged: {
            if (item) {
                item.width = bgLoader.width
            }
        }
    }

    //提示条
    Rectangle{
        width: parent.width
        height: 3
        color: rightVessel.itemShow ? TikoSeit.theme.baseTheme.borderTransition : TikoSeit.theme.baseTheme.themeTransition
        radius: 1.5
    }
}
