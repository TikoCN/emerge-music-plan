import QtQuick
import QtQuick.Window
import Tiko

TikoFrameless {
    id: inputWindow
    width: 600
    height: 400
    minimumWidth: 420
    minimumHeight: 260
    visible: false
    title: qsTr("请输入")

    property string inputText: inputLine.text
    property string orgText: ""
    signal accept()
    signal cancel()
    signal finishInput()
    signal input()

    function open() {
        show()
        requestActivate()
        Qt.callLater(function() {
            inputLine.forceActiveFocus()
            inputLine.selectAll()
        })
    }

    onActiveChanged: {
        if (active)
            inputLine.forceActiveFocus()
    }

    Item {
        id: windowSurface
        anchors.fill: parent
        anchors.margins: 10

        Rectangle {
            anchors.fill: parent
            color: TikoSeit.theme.colorBgView
            radius: 10
        }

        TikoWindow {
            id: titleBar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            title: inputWindow.title
            windowTarget: inputWindow
            onCloseRequested: inputWindow.close()
        }

        TikoTextLine {
            id: helpItem
            anchors.bottom: inputLine.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: TikoSeit.emphasizeMargins
            width: inputLine.width
            horizontalAlignment: Text.AlignHCenter
            level: 2
            text: inputWindow.title
        }

        TikoTextInput {
            id: inputLine
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: -height / 2 - parent.width / 20
            width: parent.width - 40
            placeholderText: inputWindow.orgText
            horizontalAlignment: Text.AlignHCenter
            onEditingFinished: inputWindow.finishInput()
            onTextEdited: inputWindow.input()
        }

        TikoButtonDefault {
            textLine.text: qsTr("确定")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: -width / 2 - parent.width / 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: height / 2 + parent.width / 20
            onClicked: {
                inputWindow.close()
                inputWindow.accept()
            }
        }

        TikoButtonDefault {
            textLine.text: qsTr("取消")
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: width / 2 + parent.width / 20
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: height / 2 + parent.width / 20
            onClicked: {
                inputWindow.cancel()
                inputWindow.close()
            }
        }
    }

    function setNormalText() {
        inputLine.text = orgText
    }
}
