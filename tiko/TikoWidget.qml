import QtQuick

Window{
    id: tikoWidget
    color: "#00000000"
    flags: Qt.Window | Qt.FramelessWindowHint // 尝试隐藏边框和标题栏

    property color borderColor: TikoSeit.transparentColor
    property color backColor: TikoSeit.backdropColor
    // opacity

    //圆角
    Rectangle{
        anchors.fill: parent
        topLeftRadius: 10
        topRightRadius: 10
        color: tikoWidget.backColor
        opacity: tikoWidget.opacity
    }

    Rectangle{
        anchors.fill: parent
        color: "#00000000"
        border.color: tikoWidget.backColor
        topLeftRadius: 10
        topRightRadius: 10
        border.width: 0.5
        opacity: tikoWidget.opacity
    }
}
