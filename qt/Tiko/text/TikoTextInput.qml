import QtQuick
import QtQuick.Controls.Basic
import Tiko

Item {
    id: inputText
    height: TikoSeit.normalLineHeight
    implicitWidth: 300

    property Text show: show
    property TextField inputItem: inputItem
    property color borderColor: TikoSeit.theme.baseTheme.borderTransition
    property color backColor: TikoSeit.theme.baseTheme.backgroundTransition
    property color focusColor: TikoSeit.themeColor
    signal finish
    signal input

    TikoTextLine{
        id: show
        anchors.left: inputText.left
        anchors.leftMargin: 10
        height: inputText.height
        text: qsTr("tiko的输入控件")
        width: 100
    }

    TextField{
        id: inputItem
        color: TikoSeit.theme.textTheme.normalColor
        placeholderText: qsTr("在这里输入")
        anchors.right: inputText.right
        width: 200
        height: inputText.height
        onFocusChanged: {
            if(inputItem.focus){
                mouseInAnimation.start()
            }
            else{
                mouseOutAnimation.start()
            }
        }
        onEditingFinished: finish()
        onTextEdited: input()

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
        radius: 3
    }
}
