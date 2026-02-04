import QtQuick
import Tiko

Item {
    id: rightVessel
    height: rightVessel.itemShow && show.height < (bgLoader.height || 0) ?
                (bgLoader.height || 0) + 50 : show.height + 50
    property TikoButtonNormal show: show
    property Component vessel
    property bool itemShow: true

    TikoButtonNormal {
        id: show
        x: 10
        y: 25
        autoText.textType: TikoTextLine.TextType.TITLE
        implicitWidth: 100
        opacity: 0.3
        onClickLeft: rightVessel.itemShow = !rightVessel.itemShow
    }

    // 动态加载背景组件
    Loader {
        id: bgLoader
        anchors.left: show.right
        anchors.top: show.top
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
        color: rightVessel.itemShow ? TikoSeit.themeColor : TikoSeit.transparentColor
        opacity: 0.1
        radius: 1.5
    }
}
