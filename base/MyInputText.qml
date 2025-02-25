import QtQuick
import QtQuick.Controls.Basic

Item {
    id: root
    implicitHeight: 40

    property Text show: show
    property TextField input: input

    MyAutoText{
        id: show
        height: parent.height
    }

    TextField{
        id: input
        placeholderText: qsTr("在这里输入")
        anchors.left: show.right
        anchors.leftMargin: 3
        width: parent.width - show.width - 5
        height: parent.height

        background: Rectangle{
            color: BaseSeit.transparentColor
            opacity: 0.05
        }
    }
}
