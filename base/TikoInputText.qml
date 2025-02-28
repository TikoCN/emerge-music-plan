import QtQuick
import QtQuick.Controls.Basic

Item {
    id: inputText
    implicitHeight: 40

    property Text show: show
    property TextField input: input
    property color borderColor: TikoSeit.themeColor
    property color backColor: TikoSeit.transparentColor

    TikoAutoText{
        id: show
        anchors.left: inputText.left
        anchors.leftMargin: 10
        height: inputText.height
    }

    TextField{
        id: input
        placeholderText: qsTr("在这里输入")
        anchors.right: inputText.right
        width: inputText.width - show.width - 10
        height: inputText.height

        background: Rectangle{
            color: inputText.backColor
            opacity: 0.1
            border.width: 1
            bottomRightRadius: 3
            topRightRadius: 3
        }
    }

    Rectangle{
        anchors.fill: inputText
        color: "#00000000"
        border.color: inputText.borderColor
        border.width: 1
        opacity: 0.5
        radius: 3
    }
}
