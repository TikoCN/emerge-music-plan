import QtQuick
import QtQuick.Controls.Basic
import Tiko

Item {
    id: inputText
    implicitHeight: 40
    implicitWidth: 200

    property Text show: show
    property TextField input: input
    property color borderColor: TikoSeit.transparentColor
    property color backColor: TikoSeit.transparentColor
    property color focusColor: TikoSeit.themeColor

    TikoTextLine{
        id: show
        anchors.left: inputText.left
        anchors.leftMargin: 10
        height: inputText.height
        text: qsTr("tiko的输入控件")
        width: 100
    }

    TextField{
        id: input
        placeholderText: qsTr("在这里输入")
        anchors.right: inputText.right
        width: inputText.width - show.width - 10
        height: inputText.height
        onFocusChanged: {
            if(input.focus){
                mouseInAnimation.start()
            }
            else{
                mouseOutAnimation.start()
            }
        }

        ParallelAnimation{
            id: mouseInAnimation

            ColorAnimation {
                target: background
                property: "border.color"
                to: inputText.focusColor
                duration: 200
            }

            NumberAnimation{
                target: background
                property: "border.width"
                to: 3
            }
        }

        ParallelAnimation{
            id: mouseOutAnimation

            ColorAnimation {
                target: background
                property: "border.color"
                to: inputText.borderColor
                duration: 200
            }

            NumberAnimation{
                target: background
                property: "border.width"
                to: 1
            }
        }

        background: Rectangle{
            color: inputText.backColor
            opacity: 0.1
            border.width: 1
            bottomRightRadius: 3
            topRightRadius: 3
        }
    }

    Rectangle{
        id: background
        anchors.fill: inputText
        color: "#00000000"
        border.color: inputText.borderColor
        border.width: 1
        opacity: 0.5
        radius: 3
    }
}
