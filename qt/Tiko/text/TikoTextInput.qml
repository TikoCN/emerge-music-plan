import QtQuick
import QtQuick.Controls.Basic
import Tiko

Rectangle {
    id: inputTextItem
    width: 200
    height: 50
    color: TikoSeit.theme.colorBgDefault

    property Text helpTextItem: helpTextItem
    property TextField inputItem: inputItem
    signal finish
    signal input

    TikoTextLine{
        id: helpTextItem
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.normalMargins
        anchors.verticalCenter: parent.verticalCenter
        text: qsTr("TikoTextInput")
        height: parent.height
    }

    TextField{
        id: inputItem
        color: TikoSeit.theme.colorFgDefault
        placeholderText: qsTr("insert here")
        placeholderTextColor: TikoSeit.theme.colorFgHint
        anchors.right: parent.right
        height: parent.height

        onEditingFinished: finish()
        onTextEdited: input()

        background: Rectangle{
            color: TikoSeit.theme.colorMaxTop
            radius: 5
            opacity: {
                       if (inputItem.activeFocusOnPress) return 0.12   // 按下：12% 白
                       if (inputItem.activeFocus) return 0.08   // 悬停：8% 白
                       return 0
                   }
        }
    }
}
