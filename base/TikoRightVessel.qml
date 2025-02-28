import QtQuick

Item {
    id: rightVessel
    height: rightVessel.itemShow && show.height < bgLoader.height ?
                bgLoader.height + 50 : show.height + 50
    property TikoButton show: show
    property Component vessel
    property bool itemShow: true

    TikoButton{
        id: show
        x: 10
        y: 25
        textShow.exSize: 5
        implicitWidth: 100
        opacity: 0.3
        onClicked: rightVessel.itemShow = !rightVessel.itemShow

    }

    // 动态加载背景组件
    Loader {
        id: bgLoader
        x: show.x + show.width + 10
        y: show.y
        width: parent.width - x - 10
        sourceComponent: rightVessel.vessel
        visible: rightVessel.itemShow
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
