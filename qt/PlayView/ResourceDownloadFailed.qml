import QtQuick
import QtQuick.Window
import Tiko

TikoFrameless {
    id: failureWindow
    width: 420
    height: 220
    minimumWidth: 420
    minimumHeight: 220
    visible: false
    title: qsTr("下载失败")
    flags: Qt.Window | Qt.Dialog | Qt.FramelessWindowHint

    property string message: ""

    onClosing: Qt.callLater(function() { failureWindow.destroy() })

    Item {
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
            title: failureWindow.title
            windowTarget: failureWindow
            onCloseRequested: failureWindow.close()
        }

        Column {
            anchors.top: titleBar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: TikoSeit.emphasizeMargins
            spacing: 16

            Text {
                width: parent.width
                height: parent.height - 50
                text: failureWindow.message
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                color: TikoSeit.theme.colorFgDefault
                font.family: TikoSeit.theme.fontDefault.family
                font.pointSize: TikoSeit.theme.fontDefault.pointSize
            }

            Row {
                width: parent.width
                height: 40
                layoutDirection: Qt.RightToLeft

                TikoButtonDefault {
                    textLine.text: qsTr("确定")
                    onLeftClicked: failureWindow.close()
                }
            }
        }
    }
}
