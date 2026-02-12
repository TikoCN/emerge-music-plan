import QtQuick
import QtQuick.Controls.Basic
import Tiko

Item {
    id: inputText
    height: childrenRect.height
    width: childrenRect.width

    property Text show: show
    property TextField inputItem: inputItem
    property color borderColor: TikoSeit.theme.baseTheme.borderTransition
    property color backColor: TikoSeit.theme.baseTheme.backgroundTransition
    property color focusColor: TikoSeit.theme.baseTheme.themeTransition
    signal finish
    signal input

    Rectangle{
        id: background
        color: "#00000000"
        border.color: inputText.borderColor
        border.width: 1
        radius: 3

        height: TikoSeit.normalLineHeight
        width: childrenRect.width + TikoSeit.normalMargins

        TikoTextLine{
            id: show
            anchors.left: background.left
            anchors.leftMargin: TikoSeit.normalMargins
            anchors.verticalCenter: background.verticalCenter
            text: qsTr("tiko的输入控件")
        }

        TextField{
            id: inputItem
            color: TikoSeit.theme.textTheme.normalColor
            placeholderText: qsTr("在这里输入")
            anchors.left: show.right
            anchors.leftMargin: TikoSeit.subitemSpace
            implicitWidth: 200
            height: background.height
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

            background: Rectangle{
                color: TikoSeit.theme.baseTheme.transparentNormal
                border.width: 0
                bottomRightRadius: 3
                topRightRadius: 3
            }
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
}
