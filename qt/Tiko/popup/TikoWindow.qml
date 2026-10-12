import QtQuick
import QtQuick.Window
import Tiko

Item {
    id: titleBar

    property string title: ""
    property var windowTarget: null
    signal closeRequested()

    height: 48

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onPressed: {
            if (titleBar.windowTarget !== null) {
                titleBar.windowTarget.startSystemMove()
            }
        }
    }

    TikoTextLine {
        anchors.left: parent.left
        anchors.leftMargin: TikoSeit.emphasizeMargins
        anchors.verticalCenter: parent.verticalCenter
        width: Math.max(0, parent.width - controlButtons.width - TikoSeit.emphasizeMargins * 2)
        text: titleBar.title
        level: 2
        elide: Text.ElideNone
    }

    Row {
        id: controlButtons
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.rightMargin: 8
        spacing: 4

        TikoButtonIcon {
            icon.source: "qrc:/image/min.svg"
            level: 0
            onLeftClicked: {
                if (titleBar.windowTarget !== null) {
                    titleBar.windowTarget.showMinimized()
                }
            }
        }

        TikoButtonIcon {
            icon.source: titleBar.windowTarget !== null
                         && titleBar.windowTarget.visibility === Window.Maximized
                         ? "qrc:/image/normal.svg" : "qrc:/image/max.svg"
            level: 0
            onLeftClicked: {
                if (titleBar.windowTarget === null) {
                    return
                }
                if (titleBar.windowTarget.visibility === Window.Maximized) {
                    titleBar.windowTarget.showNormal()
                } else {
                    titleBar.windowTarget.showMaximized()
                }
            }
        }

        TikoButtonIcon {
            icon.source: "qrc:/image/close.svg"
            level: 0
            onLeftClicked: titleBar.closeRequested()
        }
    }
}
